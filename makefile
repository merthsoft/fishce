# ----------------------------
# Makefile Options
# ----------------------------

NAME = FISH
ICON = icon.png
DESCRIPTION = "Fishies"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Ofast
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
