// Constants
#define ENTER_ADDR 0
#define EXIT_ADDR 4

class DataModel {
  public:
    DataModel(int loginTimeSec = 0, int logoutTimeSec = 0);

    void login();
    void login(int loginTimeSec);
    int getLoginTimeSec();

    void logout();
    void logout(int logoutTimeSec);
    int getLogoutTimeSec();

    bool atWork();
    int getAtWorkTimeSec();

  private:
    int loginTimeSec_;
    int logoutTimeSec_;
};
