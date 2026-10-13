//
// Created by Yasmeena Khalaf on 07/06/2026.
//

#ifndef DS_WET1_SPRING_2026_NODES_H
#define DS_WET1_SPRING_2026_NODES_H


#include "AVLTree.h"

struct RoomNode;
struct GuestSeatNode;
struct TableNode;
struct GuestNode {
    int guestId;
    int roomNum;
    RoomNode* roomNodePtr;
    int lastMealVisited;
    GuestSeatNode* seatNode;
    GuestNode(int id, int room) : guestId(id), roomNum(room),
    roomNodePtr(nullptr), lastMealVisited(0), seatNode(nullptr) {}
};
struct RoomNode {
    int roomNum;
    int guestId;
    RoomNode* prevRoom;
    RoomNode* nextRoom;
    RoomNode(int room, int guest): roomNum(room), guestId(guest),
    prevRoom(nullptr), nextRoom(nullptr) {}
};
struct GuestSeatNode {
    int guestId;
    TableNode* table;
    GuestNode* guestNodePtr;
    GuestSeatNode(int id, TableNode* t, GuestNode* gPtr) : guestId(id),
    table(t), guestNodePtr(gPtr) {}
};
struct TableNode {
    int tableId;
    int capacity;
    int currentSize;
    AVLTree<int, GuestSeatNode*> TableGuestsTree;
    TableNode(int id, int cap) : tableId(id), capacity(cap), currentSize(0),
          TableGuestsTree() {}
    //here I should determine if to implement ~TableGuestTree or implement it later in .cpp
};


#endif //DS_WET1_SPRING_2026_NODES_H
