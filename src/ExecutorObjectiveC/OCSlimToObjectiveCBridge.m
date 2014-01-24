#import "OCSlimToObjectiveCBridge.h"
#import "OCSObjectiveCtoCBridge.h"

NSArray* SlimListQueryTable_ToNSArray(SlimList *self) {
    NSMutableArray *table = [[NSMutableArray alloc] init];

    SlimList *tableHeaderRow = SlimList_GetListAt(self, 0);
    NSArray *tableColumnNames = SlimList_ToNSArray(tableHeaderRow);
    
    int rowIndex = 1;
    while (rowIndex < SlimList_GetLength(self)) {
        SlimList *row = SlimList_GetListAt(self, rowIndex);
        NSMutableDictionary *columnValues = [[NSMutableDictionary alloc] init];
        for (int col = 0; col < SlimList_GetLength(row); col++) {
            NSString *value = SlimList_GetNSStringAt(row, col);
            [columnValues setValue:value forKey:tableColumnNames[col]];
        }
        SlimList_Destroy(row);
        [table addObject:columnValues];
        rowIndex++;
    }

    SlimList_Destroy(tableHeaderRow);
    return table;
}