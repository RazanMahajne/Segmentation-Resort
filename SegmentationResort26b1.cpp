// You can edit anything you want in this file.
// However you need to implement all public StudentCourseManager function, as provided below as a template

#include "SegmentationResort26b1.h"


SegmentationResort::SegmentationResort() :
    GuestsTree(),
    RoomsTree(),
    TablesTree(),
    currentMealCounter(1),
    cleanCyclePointer(nullptr),
    lastRoomCleaned(0)
{
}

void SegmentationResort::freeGuests(TreeNode<int, GuestNode*>* node)
{
    if (!node)
        return;

    freeGuests(node->getLeft());
    freeGuests(node->getRight());
    delete node->getValue();
}

void SegmentationResort::freeRooms(TreeNode<int, RoomNode*>* node)
{
    if (!node)
        return;

    freeRooms(node->getLeft());
    freeRooms(node->getRight());
    delete node->getValue();
}

void SegmentationResort::freeTableSeat(TreeNode<int, GuestSeatNode*>* node)
{
    if (!node)
        return;

    freeTableSeat(node->getLeft());
    freeTableSeat(node->getRight());
    delete node->getValue();
}

void SegmentationResort::freeTables(TreeNode<int, TableNode*>* node)
{
    if (!node)
        return;

    freeTables(node->getLeft());
    freeTables(node->getRight());
    TableNode* table = node->getValue();
    if (table)
    {
        freeTableSeat(table->TableGuestsTree.getRoot());
        table->TableGuestsTree.postOrderFree();
        delete table;
    }
}

SegmentationResort::~SegmentationResort()
{
    freeGuests(GuestsTree.getRoot());
    GuestsTree.postOrderFree();

    freeRooms(RoomsTree.getRoot());
    RoomsTree.postOrderFree();

    freeTables(TablesTree.getRoot());
    TablesTree.postOrderFree();

    cleanCyclePointer = nullptr;
    currentMealCounter = 1;
}

StatusType SegmentationResort::checkIn(int guestId, int roomNum)
{
    if (guestId <= 0 || roomNum <= 0)
        return StatusType::INVALID_INPUT;

    if (GuestsTree.find(guestId) )
        return StatusType::FAILURE;

    if (RoomsTree.find(roomNum) )
        return StatusType::FAILURE;

    GuestNode* guest = nullptr;
    RoomNode* room = nullptr;

    bool guestInserted = false;
    bool roomInserted = false;

    try
    {
        guest = new GuestNode(guestId, roomNum);
        room = new RoomNode(roomNum, guestId);

        GuestsTree.insert(guestId, guest);
        guestInserted = true;

        RoomsTree.insert(roomNum, room);
        roomInserted = true;

        guest->roomNodePtr = room;
        if (!cleanCyclePointer)
        {
            room->nextRoom = room;
            room->prevRoom = room;
            cleanCyclePointer = room;
        }
        else
        {
            TreeNode<int, RoomNode*>* roomTreeNode = RoomsTree.find(roomNum);
            TreeNode<int, RoomNode*>* predNode = RoomsTree.predecessor(roomTreeNode);
            TreeNode<int, RoomNode*>* succNode = RoomsTree.successor(roomTreeNode);

            RoomNode* prevRoom =
                predNode ? predNode->getValue() : RoomsTree.findMax()->getValue();

            RoomNode* nextRoom =
                succNode ? succNode->getValue() : RoomsTree.findMin()->getValue();


            room->nextRoom = nextRoom;
            room->prevRoom = prevRoom;

            nextRoom->prevRoom = room;
            prevRoom->nextRoom = room;

            int nextToClean = cleanCyclePointer->roomNum;

            if (lastRoomCleaned < nextToClean)
            {
                if (lastRoomCleaned < roomNum && roomNum < nextToClean)
                    cleanCyclePointer = room;
            }
            else
            {
                if (roomNum > lastRoomCleaned || roomNum < nextToClean)
                    cleanCyclePointer = room;
            }
        }

        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        if (guestInserted)
            GuestsTree.remove(guestId);
        if (roomInserted)
            RoomsTree.remove(roomNum);

        delete guest;
        delete room;
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::checkOut(int guestId)
{
    try
    {
        if (guestId <= 0)
            return StatusType::INVALID_INPUT;

        TreeNode<int, GuestNode*>* guestTreeNode = GuestsTree.find(guestId);
        if (!guestTreeNode)
            return StatusType::FAILURE;

        GuestNode* guest = guestTreeNode->getValue();


        if (guest->seatNode)
            return StatusType::FAILURE;

        RoomNode* room = guest->roomNodePtr;
        if (room)
        {
            if (cleanCyclePointer == room)
            {
                if (room->nextRoom == room)
                {
                    cleanCyclePointer = nullptr;
                }
                else{
                    cleanCyclePointer = room->nextRoom;
                }
            }

            if (room->nextRoom == room)
            {
                room->nextRoom = nullptr;
                room->prevRoom = nullptr;
            }
            else
            {
                room->prevRoom->nextRoom = room->nextRoom;
                room->nextRoom->prevRoom = room->prevRoom;

                room->nextRoom = nullptr;
                room->prevRoom = nullptr;
            }

            RoomsTree.remove(room->roomNum);
            delete room;
        }
        GuestsTree.remove(guestId);
        delete guest;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::addTable(int tableId, int capacity)
{
    if (tableId <= 0 || capacity <= 0)
        return StatusType::INVALID_INPUT;
    if (TablesTree.find(tableId))
        return StatusType::FAILURE;

    TableNode* table = nullptr;
    try
    {
        table = new TableNode(tableId, capacity);
        TablesTree.insert(tableId, table);

        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        delete table;
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::removeTable(int tableId)
{
    try
    {
        if (tableId <= 0)
            return StatusType::INVALID_INPUT;
        TreeNode<int, TableNode*>* tableTreeNode = TablesTree.find(tableId);
        if (!tableTreeNode)
            return StatusType::FAILURE;
        TableNode* table = tableTreeNode->getValue();
        if (table->currentSize > 0)
            return StatusType::FAILURE;

        TablesTree.remove(table->tableId);
        delete table;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::enterDiningRoom(int guestId, int tableId)
{
    if (tableId <= 0 || guestId <= 0)
        return StatusType::INVALID_INPUT;
    TreeNode<int, TableNode*>* tableTreeNode = TablesTree.find(tableId);
    TreeNode<int, GuestNode*>* guestTreeNode = GuestsTree.find(guestId);
    if (!guestTreeNode|| !tableTreeNode)
        return StatusType::FAILURE;

    GuestNode* guest = guestTreeNode->getValue();
    TableNode* targetTable = tableTreeNode->getValue();

    if (guest->seatNode)
        return StatusType::FAILURE;
    if (guest->lastMealVisited == currentMealCounter)
        return StatusType::FAILURE;
    if (targetTable->currentSize >= targetTable->capacity)
        return StatusType::FAILURE;

    GuestSeatNode* guestSeat = nullptr;
    bool seatInserted = false;


    try
    {
        guestSeat = new GuestSeatNode(guestId, targetTable, guest);
        targetTable->TableGuestsTree.insert(guestId, guestSeat);
        seatInserted = true;

        targetTable->currentSize++;
        guest->seatNode = guestSeat;
        guest->lastMealVisited = currentMealCounter;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        if (seatInserted)
            targetTable->TableGuestsTree.remove(guestSeat->guestId);

        delete guestSeat;
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::leaveDiningRoom(int guestId, int tableId)
{
    try
    {
        if (tableId <= 0 || guestId <= 0)
            return StatusType::INVALID_INPUT;
        TreeNode<int, TableNode*>* tableTreeNode = TablesTree.find(tableId);
        if (!tableTreeNode)
            return StatusType::FAILURE;

        TableNode* table = tableTreeNode->getValue();
        TreeNode<int, GuestSeatNode*>* seatTreeNode = table->TableGuestsTree.find(guestId);
        if (!seatTreeNode)
            return StatusType::FAILURE;

        GuestSeatNode* seat = seatTreeNode->getValue();
        GuestNode* guest = seat->guestNodePtr;

        if (guest->lastMealVisited < currentMealCounter)
            guest->lastMealVisited = currentMealCounter;

        table->TableGuestsTree.remove(guestId);
        guest->seatNode = nullptr;
        table->currentSize--;
        delete seat;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::reheatFood()
{
    try
    {
        currentMealCounter++;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType SegmentationResort::joinTables(int tableId1, int tableId2)
{
    if (tableId1 <= 0 || tableId2 <= 0 || tableId1 == tableId2)
        return StatusType::INVALID_INPUT;
    TreeNode<int, TableNode*>* tableTreeNode1 = TablesTree.find(tableId1);
    TreeNode<int, TableNode*>* tableTreeNode2 = TablesTree.find(tableId2);
    if (!tableTreeNode1 || !tableTreeNode2)
        return StatusType::FAILURE;
    TableNode* table1 = tableTreeNode1->getValue();
    TableNode* table2 = tableTreeNode2->getValue();

    int totalSize = table1->currentSize + table2->currentSize;
    TreeNode<int, GuestSeatNode*>** arr1 = nullptr;
    TreeNode<int, GuestSeatNode*>** arr2 = nullptr;
    TreeNode<int, GuestSeatNode*>** arr3 = nullptr;
    try
    {
        arr1 = new TreeNode<int, GuestSeatNode*>*[table1->currentSize];
        arr2 = new TreeNode<int, GuestSeatNode*>*[table2->currentSize];
        arr3 = new TreeNode<int, GuestSeatNode*>*[totalSize];

        table1->TableGuestsTree.inOrderToArray(arr1);
        table2->TableGuestsTree.inOrderToArray(arr2);

        int i = 0, j = 0, k = 0;
        while (i < table1->currentSize && j < table2->currentSize)
        {
            if (arr1[i]->getKey() < arr2[j]->getKey())
                arr3[k++] = arr1[i++];
            else
                arr3[k++] = arr2[j++];
        }
        while (i < table1->currentSize)
            arr3[k++] = arr1[i++];
        while (j < table2->currentSize)
            arr3[k++] = arr2[j++];

        for (i = 0; i < totalSize; i++)
        {
            GuestSeatNode* seat = arr3[i]->getValue();

            seat->table = table1;
            seat->guestNodePtr->seatNode = seat;
        }
        table1->capacity += table2->capacity;
        table1->currentSize = totalSize;

        table1->TableGuestsTree.buildFromSortedArray(arr3, totalSize);

        table2->TableGuestsTree.buildFromSortedArray(nullptr, 0);
        TablesTree.remove(tableId2);
        delete table2;

        delete[] arr1;
        delete[] arr2;
        delete[] arr3;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        delete[] arr1;
        delete[] arr2;
        delete[] arr3;
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> SegmentationResort::joinFriend(int guestId1, int guestId2)
{
    try
    {
        if (guestId1 <= 0 || guestId2 <= 0 || guestId1 == guestId2)
            return StatusType::INVALID_INPUT;

        TreeNode<int, GuestNode*>* guestTreeNode1 = GuestsTree.find(guestId1);
        TreeNode<int, GuestNode*>* guestTreeNode2 = GuestsTree.find(guestId2);
        if (!guestTreeNode1|| !guestTreeNode2)
            return StatusType::FAILURE;

        GuestNode* guest1 = guestTreeNode1->getValue();
        GuestNode* guest2 = guestTreeNode2->getValue();
        if (!guest2->seatNode)
            return StatusType::FAILURE;

        TableNode* targetTable = guest2->seatNode->table;
        if (targetTable->currentSize >= targetTable->capacity)
            return StatusType::FAILURE;

        if (guest1->seatNode)
            return StatusType::FAILURE;

        if (guest1->lastMealVisited == currentMealCounter)
            return StatusType::FAILURE;

        StatusType result = enterDiningRoom(guestId1, targetTable->tableId);

        if (result == StatusType::SUCCESS)
            return targetTable->tableId;

        return result;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> SegmentationResort::cleanNextRoom()
{
    try
    {
        if (!cleanCyclePointer)
            return StatusType::FAILURE;

        int roomToClean = cleanCyclePointer->roomNum;
        lastRoomCleaned = roomToClean;
        cleanCyclePointer = cleanCyclePointer->nextRoom;

        return roomToClean;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}
