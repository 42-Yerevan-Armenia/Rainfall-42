class N {
    public:
        N(int);
        void setAnnotation(char*);
        virtual void someVirtualFunc();
    };
    
    int main(int argc, char** argv) {
        if (argc <= 1) {
            _exit(1);
        }
    
        N* n1 = new N(0x6c);    // 0x6c = 108 decimal
        N* n2 = new N(0x6);     // 0x6 = 6 decimal
    
        n1->setAnnotation(argv[1] + 1);
        n2->someVirtualFunc();
    
        return 0;
    }
    