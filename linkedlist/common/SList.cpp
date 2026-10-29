/*
 * SList.cpp -- provided implementation. You do not need to edit this.
 */

#include "SList.h"

#include <sstream>
#include <stdexcept>
#include <unordered_set>

int &SList::liveNodes() {
    static int count = 0;
    return count;
}

int SList::liveNodeCount() {
    return liveNodes();
}

SList::SList() : front(nullptr), numItems(0) {}

SList::SList(std::initializer_list<int> values) : SList() {
    Node **tail = &front;
    for (int v : values) {
        *tail = new Node(v, nullptr);
        tail = &(*tail)->next;
        numItems++;
    }
}

SList::SList(const std::vector<int> &values) : SList() {
    Node **tail = &front;
    for (int v : values) {
        *tail = new Node(v, nullptr);
        tail = &(*tail)->next;
        numItems++;
    }
}

/*
 * Deliberately defensive: if a bug left the list with a cycle, a plain
 * "delete until nullptr" loop would never stop (or double-delete), which
 * would hide the real test failure. So we stop at any node seen before.
 */
SList::~SList() {
    std::unordered_set<Node *> seen;
    std::vector<Node *> toDelete;
    for (Node *curr = front; curr != nullptr && seen.insert(curr).second;
         curr = curr->next) {
        toDelete.push_back(curr);
    }
    for (Node *n : toDelete) delete n;
}

int SList::size() const {
    return numItems;
}

bool SList::isEmpty() const {
    return numItems == 0;
}

void SList::pushAtFront(int value) {
    front = new Node(value, front);
    numItems++;
}

void SList::pushAtBack(int value) {
    Node **tail = &front;
    while (*tail != nullptr) tail = &(*tail)->next;
    *tail = new Node(value, nullptr);
    numItems++;
}

std::vector<const SList::Node *> SList::walk() const {
    std::vector<const Node *> nodes;
    const Node *curr = front;
    while (curr != nullptr) {
        if (static_cast<int>(nodes.size()) == numItems) {
            throw std::logic_error(
                "broken list: there are MORE nodes than size() says (" +
                std::to_string(numItems) +
                "). Is the last node's next nullptr? Is there a cycle? "
                "Did you update numItems?");
        }
        nodes.push_back(curr);
        curr = curr->next;
    }
    if (static_cast<int>(nodes.size()) != numItems) {
        throw std::logic_error(
            "broken list: found only " + std::to_string(nodes.size()) +
            " nodes but size() says " + std::to_string(numItems) +
            ". Did you lose part of the list, or forget to update numItems?");
    }
    return nodes;
}

std::vector<int> SList::toVector() const {
    std::vector<int> out;
    for (const Node *n : walk()) out.push_back(n->data);
    return out;
}

std::string SList::toString() const {
    std::stringstream ss;
    ss << "[";
    std::vector<int> values = toVector();
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) ss << ", ";
        ss << values[i];
    }
    ss << "]";
    return ss.str();
}

std::vector<const void *> SList::nodeAddresses() const {
    std::vector<const void *> out;
    for (const Node *n : walk()) out.push_back(n);
    return out;
}
