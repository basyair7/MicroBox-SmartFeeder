#include "main.h"

class MyProgram {
public:
    static MyProgram& instance() {
        static MyProgram instance;
        return instance;
    }

    static void mainSetup() {
        instance().__setup__();
    }

    static void mainLoop() {
        instance().__loop__();
    }

private:
    void __setup__() {

    }

    void __loop__() {
        
    }
};


void setup() { MyProgram::mainSetup(); }
void loop()  { MyProgram::mainLoop(); }