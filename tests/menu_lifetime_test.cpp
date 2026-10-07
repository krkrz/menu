#include "MenuItemIntf.h"
#include <cstdio>
#include <vector>

// The lifetime test does not initialize a TJS host or dispatch menu clicks.
const tjs_char* TVPMenuIDOverflow = L"Menu ID overflow";
void tTJSNI_MenuItem::MenuItemClick() {}

static bool require(bool condition, const char* message) {
    if(!condition) std::fprintf(stderr, "%s\n", message);
    return condition;
}

int main() {
    // Exercise the new[] backup array under locked removal.
    int a = 1, b = 2;
    tVoidObjectList<int> list;
    list.Add(&a);
    list.Add(&b);
    list.SafeLock();
    list.Remove(&a);
    list.SafeUnlock();
    if(!require(list.GetActualCount() == 1, "locked list removal failed")) return 1;

    // More than the available command IDs, and thousands of detached HMENUs.
    for(int round = 0; round < 20; ++round) {
        WindowMenuItem root(NULL, NULL);
        if(!require(::IsMenu(root.GetHandle()), "root HMENU allocation failed")) return 1;
        std::vector<WindowMenuItem*> items;
        std::vector<HMENU> handles;
        for(int i = 0; i < 160; ++i) {
            WindowMenuItem* item = new WindowMenuItem(NULL, NULL);
            if(!require(::IsMenu(item->GetHandle()), "item HMENU allocation failed")) return 1;
            handles.push_back(item->GetHandle());
            root.Add(item);
            items.push_back(item);
        }
        for(size_t i = 0; i < items.size(); ++i) {
            delete items[i];
            if(!require(!::IsMenu(handles[i]), "detached item HMENU leaked")) return 1;
        }
        if(!require(::GetMenuItemCount(root.GetHandle()) == 0, "parent still contains items")) return 1;
    }

    // Continue past the command-ID range without rebuilding a large menu tree.
    for(int i = 0; i < 28800; ++i) {
        WindowMenuItem* item = new WindowMenuItem(NULL, NULL);
        HMENU handle = item->GetHandle();
        if(!require(::IsMenu(handle), "HMENU allocation failed during ID recycling")) return 1;
        delete item;
        if(!require(!::IsMenu(handle), "leaf HMENU leaked")) return 1;
    }

    // Destroying a parent must leave a detached child alive and usable.
    WindowMenuItem* parent = new WindowMenuItem(NULL, NULL);
    WindowMenuItem* child = new WindowMenuItem(NULL, NULL);
    HMENU parentHandle = parent->GetHandle(), childHandle = child->GetHandle();
    parent->Add(child);
    delete parent;
    if(!require(!::IsMenu(parentHandle) && ::IsMenu(childHandle), "parent/child handle ownership failed")) return 1;
    delete child;
    if(!require(!::IsMenu(childHandle), "child HMENU leaked")) return 1;
    std::puts("menu lifetime: 32,000 item creations passed");
    return 0;
}
