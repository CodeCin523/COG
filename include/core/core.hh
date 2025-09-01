#ifndef __CORE_HH__
#define __CORE_HH__

class Core {
public:
    Core();
    ~Core();

    /* Disabling copy and move semantics */
    Core& operator=(const Core&) = delete;
    Core(const Core&) = delete;
    Core& operator=(Core&&) = delete;
    Core(Core&&) = delete;
};

#endif