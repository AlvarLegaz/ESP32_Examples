#ifndef APPPROGRAM_H
#define APPPROGRAM_H

class AppProgram {
public:
    void init();
    void run();
    void stop();

private:
    void handleButtonPress(int buttonPin, int ledPin, int button_mode);
    bool isRunning_;
    int button_mode;
    void error();
};

#endif // APPPROGRAM_H
