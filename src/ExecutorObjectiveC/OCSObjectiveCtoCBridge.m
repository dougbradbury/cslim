#import "OCSObjectiveCtoCBridge.h"
#import "SlimList.h"
#import "SlimListSerializer.h"
#import <objc/runtime.h>




char* NSStringToCString(NSString* string) {
    return (char*)[string UTF8String];
}

NSString* CStringToNSString(char const* cString) {
    return [NSString stringWithUTF8String: cString];
}

NSArray* SlimList_ToNSArray(SlimList* self) {
    int length = SlimList_GetLength(self);
    NSMutableArray* array = [NSMutableArray array];
    for(int i=0; i<length; i++) {
        [array addObject: SlimList_GetNSStringAt(self, i)];
    }
    return array;
}

NSString* SlimList_GetNSStringAt(SlimList* self, int index) {
    return CStringToNSString(SlimList_GetStringAt(self, index));
}





void amendSlimListForSinglePropertyObject(id object, SlimList ** record) {
    SlimList * property = SlimList_Create();
    // key
    SlimList_AddString(property, "property");
    // value
    if (isBOOLObject(object)) {
        SlimList_AddString(property, NSStringToCString( stringForBOOLObject(object) ));
    }
    else {
        SlimList_AddString(property, NSStringToCString([NSString stringWithFormat:@"%@", object]));
    }
    SlimList_AddList(*record, property);
}





NSString * getPropertyType(objc_property_t property)
{
    const char * propAttr = property_getAttributes(property);
    NSString *propString = [NSString stringWithUTF8String:propAttr];
    NSArray *attrArray = [propString componentsSeparatedByString:@","];
    if (attrArray && attrArray.count > 0) {
        return attrArray[0];
    }
    return @"";
}


void slimListAddPropertyValue(id object, objc_property_t property, SlimList ** slimList) {

    NSString * propertyName = [[NSString alloc] initWithUTF8String:property_getName(property)];
    
    if (propertyName) {
        
        id propertyValue = [object valueForKey:propertyName];
        
        if (propertyValue) {
            NSString * propertyType = getPropertyType(property);
            if ([propertyType isEqualToString:@"Tc"]) {
                if (((NSNumber *)propertyValue).boolValue) {
                    SlimList_AddString(*slimList, NSStringToCString(@"true"));
                } else {
                    SlimList_AddString(*slimList, NSStringToCString(@"false"));
                }
            }
            else if([propertyType isEqualToString:@"T@\"NSNumber\""] && isBOOLObject(propertyValue)) {
                SlimList_AddString(*slimList, NSStringToCString(stringForBOOLObject(propertyValue)));
                
            }
            else {
                SlimList_AddString(*slimList, NSStringToCString([NSString stringWithFormat:@"%@", propertyValue]));
            }
        }
    }
}





void slimListAddPropertyKey(objc_property_t property, SlimList ** slimList) {
    
    NSString * propertyName = [[NSString alloc] initWithUTF8String:property_getName(property)];
    SlimList_AddString(*slimList, NSStringToCString(propertyName));
}



void amendSlimListForMultiplePropertyObject(id object, objc_property_t * properties, unsigned int maxProperties, SlimList ** aRecord) {
    
    for (unsigned int propertyCount=0; propertyCount < maxProperties; propertyCount++) {
        SlimList * slimListProperty = SlimList_Create();
        // key
        objc_property_t property = properties[propertyCount];
        slimListAddPropertyKey( property, &slimListProperty );
        // value
        slimListAddPropertyValue( object, property, &slimListProperty );
        SlimList_AddList(*aRecord, slimListProperty);
    }
}






NSString * SerializeNSStringFromNSArray(NSArray* array) {
    
    SlimList* records = SlimList_Create();
    
    for (id object in array) {
        
        SlimList * aRecord = SlimList_Create();
        
        unsigned int maxProperties;
        objc_property_t * properties = class_copyPropertyList([object class], &maxProperties);
        
        if (maxProperties > 0) {
            amendSlimListForMultiplePropertyObject(object, properties, maxProperties, &aRecord);
        }
        else {
            amendSlimListForSinglePropertyObject( object, &aRecord );
        }

        SlimList_AddList(records, aRecord);
    }
    
    NSString * output = [NSString stringWithUTF8String: SlimList_Serialize(records)];
    return output;
}








NSString * stringForBOOLObject(NSNumber * boolNumber) {
    
    if (((NSNumber *)boolNumber).boolValue) {
        return @"true";
    }
    else {
        return @"false";
    }
}



BOOL isBOOLObject(id object) {
    
    NSString * classString = NSStringFromClass([object class]);
    if ([classString isEqualToString:@"__NSCFBoolean"]) {
        return YES;
    }
    return NO;
}
