struct point {
    float x, y, z;
};
struct face {
    int p1, p2, p3;
};
class master {
public:
    master();
    ~master();
    int main(int argc, char const *argv[]);
};