#include <Particle.h>

#include "DataModel.h"

DataModel::DataModel(int loginEEPROMAddress, int logoutEEPROMAddress) {
  loginEEPROMAddress_ = loginEEPROMAddress;
  logoutEEPROMAddress_ = logoutEEPROMAddress;

  login(loadEEPROM(loginEEPROMAddress));
  logout(loadEEPROM(logoutEEPROMAddress));
}

void DataModel::login() {
  DataModel::login(Time.now());
}
void DataModel::login(int loginTimeSec) {
  if (loginTimeSec > 1483228800 && loginTimeSec < 4102444800) {
      loginTimeSec_ = loginTimeSec;
      Particle.publish("work_state", "Start|||" + Time.format(loginTimeSec_, "%d.%m.%Y %H:%M"));
      EEPROM.put(loginEEPROMAddress_, loginTimeSec_);
  }
}
int DataModel::getLoginTimeSec() {
  return loginTimeSec_;
}

void DataModel::logout() {
  logout(Time.now());
}
void DataModel::logout(int logoutTimeSec) {
  if (logoutTimeSec > 1483228800 && logoutTimeSec < 4102444800) {
      logoutTimeSec_ = logoutTimeSec;
      Particle.publish("work_state", "Ende|||" + Time.format(logoutTimeSec_, "%d.%m.%Y %H:%M"));
      EEPROM.put(logoutEEPROMAddress_, logoutTimeSec_);
  }
}
int DataModel::getLogoutTimeSec() {
  return logoutTimeSec_;
}

bool DataModel::atWork() {
  return loginTimeSec_ > logoutTimeSec_;
}

int DataModel::getAtWorkTimeSec() {
  return Time.now() - loginTimeSec_;
}

int DataModel::loadEEPROM(int address) {
    int tmpVal;
    EEPROM.get(address, tmpVal);
    return tmpVal;
}
