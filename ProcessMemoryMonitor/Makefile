# 编译器设置
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -fPIC

# 项目路径设置
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# 可执行文件和库名称
TARGET = $(BIN_DIR)/process_memory_monitor
STATIC_LIB = $(LIB_DIR)/libprocessmemorymonitor.a
SHARED_LIB = $(LIB_DIR)/libprocessmemorymonitor.so

# 源文件和目标文件
LIBRARY_SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
MAIN_SRC = $(SRC_DIR)/main.cpp

LIBRARY_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIBRARY_SRCS))
MAIN_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAIN_SRC))

# 包含目录
INCLUDES = -I$(INC_DIR)

# 编译规则
all: $(TARGET) $(STATIC_LIB) $(SHARED_LIB)

# 直接编译源代码的可执行文件
$(TARGET): $(MAIN_OBJ) $(LIBRARY_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# 静态库
$(STATIC_LIB): $(LIBRARY_OBJS) | $(LIB_DIR)
	ar rcs $@ $^

# 动态库
$(SHARED_LIB): $(LIBRARY_OBJS) | $(LIB_DIR)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

# 使用静态库的测试程序
test-static: $(STATIC_LIB) $(MAIN_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BIN_DIR)/$@ $(MAIN_OBJ) -L$(LIB_DIR) -Wl,-Bstatic -lprocessmemorymonitor -Wl,-Bdynamic

# 使用动态库的测试程序
test-shared: $(SHARED_LIB) $(MAIN_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BIN_DIR)/$@ $(MAIN_OBJ) -L$(LIB_DIR) -lprocessmemorymonitor

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

run: $(TARGET)
	./$(TARGET)

# 运行使用静态库的测试程序
run-static: $(BIN_DIR)/test-static
	./$(BIN_DIR)/test-static

# 运行使用动态库的测试程序
run-shared: $(BIN_DIR)/test-shared
	export LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH && \
	./$(BIN_DIR)/test-shared