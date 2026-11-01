# =============================================================================
# Makefile - Expression Evaluator / Balanced-Parentheses Checker
#   Linux  : make | make run | make test | make clean
#   Windows: mingw32-make | mingw32-make run | mingw32-make test | mingw32-make clean
# Author: Aksh Rastogi (02518015625)
# =============================================================================

CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude

# Platform differences: executable suffix, delete command, run prefix.
# On Windows the recipes always run in cmd.exe, whichever terminal started make.
ifeq ($(OS),Windows_NT)
    SHELL   := cmd.exe
    EXE     := .exe
    RUN     := .\\
    RM_FILE  = if exist $(1) del /Q /F $(1)
else
    EXE     :=
    RUN     := ./
    RM_FILE  = rm -f $(1)
endif

APP  := expr_eval$(EXE)
TEST := test_runner$(EXE)

# Algorithm modules (everything except the menu and main)
CORE_SRC := src/ExprCommon.cpp src/TraceTable.cpp \
            src/InfixToPostfix.cpp src/PostfixEvaluator.cpp \
            src/ParenChecker.cpp src/InfixToPrefix.cpp src/PrefixEvaluator.cpp
APP_SRC  := src/main.cpp src/Menu.cpp $(CORE_SRC)
TEST_SRC := tests/test_main.cpp $(CORE_SRC)
HEADERS  := $(wildcard include/*.h)

.PHONY: all run test clean

all: $(APP)

$(APP): $(APP_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(APP_SRC) -o $@

$(TEST): $(TEST_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $@

run: $(APP)
	$(RUN)$(APP)

test: $(TEST)
	$(RUN)$(TEST)

clean:
	$(call RM_FILE,$(APP))
	$(call RM_FILE,$(TEST))
