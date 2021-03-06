#ifndef AXLIB_DISPLAY_H
#define AXLIB_DISPLAY_H

#include <Carbon/Carbon.h>
#include <string>
#include <map>


/* NOTE(koekeishiya): User controlled spaces */
#define kCGSSpaceUser 0

/* NOTE(koekeishiya): System controlled spaces (dashboard) */
#define kCGSSpaceSystem 2

/* NOTE(koekeishiya): Fullscreen applications */
#define kCGSSpaceFullscreen 4

enum CGSSpaceSelector
{
    kCGSSpaceCurrent = 5,
    kCGSSpaceOther = 6,
    kCGSSpaceAll = 7
};

typedef int CGSSpaceID;
typedef int CGSSpaceType;

struct ax_window;
enum ax_space_flags
{
    AXSpace_DeminimizedTransition = (1 << 0),
    AXSpace_NeedsUpdate = (1 << 1),
    AXSpace_FastTransition = (1 << 2),
};

struct ax_space
{
    CFStringRef Identifier;
    CGSSpaceID ID;
    CGSSpaceType Type;
    uint32_t Flags;
    uint32_t FocusedWindow;
};

struct ax_display
{
    unsigned int ArrangementID;
    CFStringRef Identifier;
    CGDirectDisplayID ID;
    CGRect Frame;

    ax_space *Space;
    ax_space *PrevSpace;
    std::map<CGSSpaceID, ax_space> Spaces;
};

inline bool
AXLibHasFlags(ax_space *Space, uint32_t Flag)
{
    bool Result = Space->Flags & Flag;
    return Result;
}

inline void
AXLibAddFlags(ax_space *Space, uint32_t Flag)
{
    Space->Flags |= Flag;
}

inline void
AXLibClearFlags(ax_space *Space, uint32_t Flag)
{
    Space->Flags &= ~Flag;
}


void AXLibInitializeDisplays(std::map<CGDirectDisplayID, ax_display> *AXDisplays);
ax_space *AXLibGetActiveSpace(ax_display *Display);
bool AXLibIsSpaceTransitionInProgress();
bool AXLibDisplayHasSeparateSpaces();

ax_display *AXLibMainDisplay();
ax_display *AXLibCursorDisplay();
ax_display *AXLibWindowDisplay(ax_window *Window);
ax_display *AXLibSpaceDisplay(CGSSpaceID SpaceID);
ax_display *AXLibNextDisplay(ax_display *Display);
ax_display *AXLibPreviousDisplay(ax_display *Display);
ax_display *AXLibArrangementDisplay(unsigned int ArrangementID);

unsigned int AXLibDisplaySpacesCount(ax_display *Display);
unsigned int AXLibDesktopIDFromCGSSpaceID(ax_display *Display, CGSSpaceID SpaceID);
CGSSpaceID AXLibCGSSpaceIDFromDesktopID(ax_display *Display, unsigned int DesktopID);
void AXLibSpaceTransition(ax_display *Display, CGSSpaceID SpaceID);
bool AXLibSpaceHasWindow(ax_window *Window, CGSSpaceID SpaceID);
void AXLibSpaceAddWindow(CGSSpaceID SpaceID, uint32_t WindowID);
void AXLibSpaceRemoveWindow(CGSSpaceID SpaceID, uint32_t WindowID);

#endif
