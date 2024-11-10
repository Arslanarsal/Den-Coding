#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class LRUCache
{
public:
    class ListNode
    {
    public:
        int cacheKey;
        string cacheValue;
        ListNode *nextNode, *prevNode;
        ListNode(int cacheKey, string cacheValue)
        {
            this->cacheKey = cacheKey;
            this->cacheValue = cacheValue;
            nextNode = NULL;
            prevNode = NULL;
        }
    };

    int maxCapacity;
    unordered_map<int, ListNode *> cacheMap;
    ListNode *front = new ListNode(-1, "");
    ListNode *rear = new ListNode(-1, "");

    LRUCache(int maxCapacity)
    {
        this->maxCapacity = maxCapacity;
        front->nextNode = rear;
        rear->prevNode = front;
        cout << "Initialized cache with capacity " << maxCapacity << endl;
    }

    void removeNode(ListNode *node)
    {
        node->prevNode->nextNode = node->nextNode;
        node->nextNode->prevNode = node->prevNode;
    }

    void insertToFront(ListNode *node)
    {
        ListNode *tempNode = front->nextNode;
        front->nextNode = node;
        node->prevNode = front;
        node->nextNode = tempNode;
        tempNode->prevNode = node;
    }

    string fetch(int key)
    {
        if (cacheMap.find(key) == cacheMap.end())
            return "-1";
        else
        {
            ListNode *currentNode = cacheMap[key];
            removeNode(currentNode);
            insertToFront(currentNode);
            cout << "Fetch " << key << ": \"" << currentNode->cacheValue << "\" -> Cache updated to ";
            displayCache();
            return currentNode->cacheValue;
        }
    }

    void add(int key, string value)
    {
        if (cacheMap.find(key) != cacheMap.end())
        {
            ListNode *existingNode = cacheMap[key];
            existingNode->cacheValue = value;
            removeNode(existingNode);
            insertToFront(existingNode);
        }
        else
        {
            ListNode *newNode = new ListNode(key, value);
            if (cacheMap.size() == maxCapacity)
            {
                ListNode *toDelete = rear->prevNode;
                cacheMap.erase(toDelete->cacheKey);
                removeNode(toDelete);
                insertToFront(newNode);
                cacheMap[key] = newNode;
                delete toDelete;
            }
            else
            {
                insertToFront(newNode);
                cacheMap[key] = newNode;
            }
        }
        cout << "Add " << key << ": Cache = ";
        displayCache();
    }

    void displayCache()
    {
        ListNode *current = front->nextNode;
        cout << "[";
        while (current != rear)
        {
            cout << "\"" << current->cacheValue << "\"";
            if (current->nextNode != rear)
                cout << ", ";
            current = current->nextNode;
        }
        cout << "]" << endl;
    }
};

int main()
{
    LRUCache cache(3);
    cout << "\n";
    cache.add(1, "Data1");
    cache.add(2, "Data2");
    cache.fetch(1);
    cache.add(3, "Data3");
    cache.add(4, "Data4");
    cout << "Fetch 2: " << cache.fetch(2) << endl;
    cache.displayCache();
     cout << "\n";

    return 0;
}
