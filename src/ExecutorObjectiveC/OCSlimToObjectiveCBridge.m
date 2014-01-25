#import "OCSlimToObjectiveCBridge.h"
#import "OCSObjectiveCtoCBridge.h"
#import "SlimListSerializer.h"

NSArray* SlimListQueryTable_ToNSArray(SlimList *self) {
    NSMutableArray *table = [[NSMutableArray alloc] init];
    int rowIndex = 1;
    
    while (rowIndex < SlimList_GetLength(self)) {
        
        SlimList *row = SlimList_GetListAt(self, rowIndex);
        NSMutableDictionary *columnValues = [[NSMutableDictionary alloc] init];

        int numberOfColumns = SlimList_GetLength(row);
        for (int colIndex = 0; colIndex < numberOfColumns; colIndex++) {
            SlimList *cell = SlimList_GetListAt(row, colIndex);
            NSString *key = SlimList_GetNSStringAt(cell, 0);
            NSString *value = SlimList_GetNSStringAt(cell, 1);
            [columnValues setValue:value forKey:key];
        }
        
        SlimList_Destroy(row);
        [table addObject:columnValues];
        rowIndex++;
    }
    
    return [NSArray arrayWithArray:table];
}
