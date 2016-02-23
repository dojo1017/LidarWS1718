struct position
{
    int x,y,z;
};

class compassController {
public:
    compassController();
    ~compassController();

    void init();

    position getPostition();
};