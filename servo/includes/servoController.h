class servoController {
public:
    servoController(int steps);
    ~servoController();

    void nextStep();

    bool isFinish();
};