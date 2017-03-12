class DataModel {
  public:
    // Uses the EEPROM addresses to restore login/logout data from memory
    DataModel(int loginEEPROMAddress, int logoutEEPROMAddress);

    void login();
    int getLoginTimeSec();

    void logout();
    int getLogoutTimeSec();

    bool atWork();
    int getAtWorkTimeSec();

  private:
    int loginTimeSec_;
    int logoutTimeSec_;
    int loginEEPROMAddress_;
    int logoutEEPROMAddress_;

    void login(int loginTimeSec);
    void logout(int logoutTimeSec);
    int loadEEPROM(int);
};
