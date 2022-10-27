#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <type_traits>
#include <vector>
#define brigFile "brigades.dat"
#define planBrig "planBrigade.dat"
#define planReq "planRequest.dat"
#define householderName "householderName.bin"
#define requestFile "request.bin"

using namespace std;
/*point 1, 2*/
/*point 17*/

class Person {
protected:
  string name;

public:
  /*point 4*/
  Person(string name_prsn) { name = name_prsn; }
  Person(const Person &prsn) { name = prsn.name; }
  Person() {}
};

class Householder;
class Request {
private:
  int count = 0;
  int id;
  string typeOfWork;
  string size;
  float time;
  string hh_name;

public:
  Request() { /*cout << "\nits request";*/
  }

  /*getters*/
  int getId() { return id; }
  string getKindWork() { return typeOfWork; }
  string getSize() { return size; }
  float getTime() { return time; }
  int getCount() { return count; }
  string getHouseholderName() { return hh_name; }

  /*setters*/
  void setCount(int n) { count = n; }
  void setId(int n) { id = n; }
  void setWork(string work) { typeOfWork = work; }
  void setSize(string size_s) { size = size_s; }
  void setTime(float time_s) { time = time_s; }
  void setHouseholderName(string name) { hh_name = name; }
  void setInform(int n, string work, string size_s, float time_s) {
    id = n;
    typeOfWork = work;
    size = size_s;
    time = time_s;
  }
};

class Householder : Person {
private:
  /*point 3*/
  /*point 14, скорочений*/
  /*point 19*/
  vector<Request> req_list;
  string personName;
  int reqCount = 0;

  static void writeInteger(const int &id, ostream &f) {
    f.write((char *)&id, sizeof(int));
  }

  static void writeStr(const string &string, ostream &f) {
    size_t len = string.length();
    f.write((char *)&len, sizeof(size_t));
    f.write(string.data(), len);
    // cout << "success";
  }

  static void writeFloat(const float &time, ostream &f) {
    f.write((char *)&time, sizeof(float));
  }

  static int readInteger(istream &f) {
    int id;
    f.read((char *)&id, sizeof(int));
    return id;
  }

  static string readStr(istream &f) {
    size_t l;
    string work;

    f.read((char *)&l, sizeof(size_t));
    char *str = new char[l + 1];
    f.read(str, l);
    str[l] = 0;
    work = str;
    // cout << work;
    delete[] str;
    // cout << "success\n";
    return work;
  }

  static float readFloat(istream &f) {
    float time;
    f.read((char *)&time, sizeof(float));
    return time;
  }

public:
  Householder() { /* cout << "its householder";*/
  }
  /*point 5*/
  Householder(string hh_name) : Person(hh_name) {}

  /*work with files*/
  void writeInt(int &id, ostream &f) const { writeInteger(id, f); }
  int readInt(istream &f) { return readInteger(f); }
  void write(string &s, ostream &f) const { writeStr(s, f); }
  string read(istream &f) { return readStr(f); }
  void writeFl(float &time, ostream &f) const { writeFloat(time, f); }
  float readFl(istream &f) { return readFloat(f); }

  /*setters*/
  void setReqCount(int n) { reqCount = n; }
  /*point 16, 19*/
  void setReq(Request obj) { req_list.push_back(obj); }
  void setName() { personName = name; }
  void setName(string name) { personName = this->name; }

  /*getters*/
  int getReqCount() { return reqCount; }
  Request getReq(int n) { return req_list.at(n); }
  string getName() { return name; }

  vector<Request> getReq() { return req_list; }

  void deleteRequest(int n) { req_list.erase(req_list.begin() + n); }

  void display() {
    vector<Request>::iterator iter;
    int i;
    if (!req_list.empty()) {
      cout << "Householder: " << getReq()[0].getHouseholderName();
      for (iter = req_list.begin(), i = 0; iter != req_list.end();
           ++iter, i++) {
        cout << "\nThe request №" << getReq(i).getId() << ":" << endl;
        cout << "Work: " << getReq(i).getKindWork() << endl;
        cout << "Size: " << getReq(i).getSize() << endl;
        cout << "Time: " << getReq(i).getTime() << endl;
      }
    } else
      cout << "There is no requests!";
  }
};

class Dispetcher : Person {
  struct Brigade {
    bool status = 0;
    int id = 0;
    // char type;
  };
  vector<Brigade> brigade;

  struct planOfWork {
    struct Brigade brigade_p;
    Request request;
    string name = request.getHouseholderName();
  };
  planOfWork plan;
  // bool empty;

public:
  Dispetcher(){};

  /*work with files*/
  void save(ostream &put_brigades) {
    // ofstream put_brigades("brigades.dat", ios::binary | ios::out|mode);
    size_t size = brigade.size();
    for (int i = 0; i < brigade.size(); i++) {
      put_brigades.write(reinterpret_cast<const char *>(&brigade[i]),
                         brigade.size() * sizeof(Brigade));
    }
    // // put_brigades.close();
    // cout << "success";
  }

  void read() {
    ifstream getSize(brigFile, ios::ate | ios::binary);
    size_t size_o = getSize.tellg();
    getSize.close();
    // cout << size_o;

    ifstream get(brigFile, ios::binary);
    size_t size = size_o / sizeof(brigade[0]);
    // size = sqrt(size);
    // cout << size;
    vector<Brigade> bri(size);
    for (int i = 0; i < sqrt(size); i++) {
      get.read(reinterpret_cast<char *>(&bri[i]), size * sizeof(Brigade));
      createBrigade(bri[i]);
    }
    get.close();
  }

  void savePlanBrigade(Brigade &brigada) {
    ofstream putPlan(planBrig, ios::binary | ios::out);
    putPlan.write((char *)&brigada, sizeof(brigada));
    putPlan.close();
    // cout << "success";
  }

  void readPlanBrigade() {
    Brigade brigada;
    ifstream getPlan(planBrig, ios::binary | ios::in);
    getPlan.read((char *)&(brigada), sizeof(brigada));
    getPlan.close();
  }

  void savePlaneRequest(Householder &hh, Request &req) {
    int id;
    string work, size;
    float time;
    id = req.getId();
    work = req.getKindWork();
    size = req.getSize();
    time = req.getTime();

    ofstream out(planReq, ios::binary | ios::app);
    hh.writeInt(id, out);
    hh.write(work, out);
    hh.write(size, out);
    hh.writeFl(time, out);
    out.close();
  }

  void readPlaneRequest() {
    Householder hh;
    Request req;
    int id;
    string work, size;
    float time;
    ifstream in(planReq, ios::binary);

    id = hh.readInt(in);
    work = hh.read(in);
    size = hh.read(in);
    time = hh.readFl(in);
    req.setInform(id, work, size, time);
    setReqToPlan(req);
  }

  void deleteFile(const char *filename) {
    ofstream deleteSomeFile(filename, ios::trunc);
    deleteSomeFile.close();
  }

  /*point 7*/

  /*setters*/
  void setBrig(bool status_b, int id_b) {
    Brigade brig;
    brig.status = status_b;
    brig.id = id_b;
    createBrigade(brig);
  }
  void setRequest(Request obj) { plan.request = obj; }
  void createBrigade(Brigade br) { brigade.push_back(br); }
  void changeStatus(int n, bool status_b) { brigade.at(n).status = status_b; }
  /*point 18*/
  void setReqToPlan(Request req) { plan.request = req; }
  void setRequestToPlan(int n, Householder hh) {
    plan.request = hh.getReq(n);
    savePlaneRequest(hh, plan.request);
  }
  void setBrigadeToPlan(int n) {
    plan.brigade_p = brigade[n];
    savePlanBrigade(brigade[n]);
  }

  /*getters*/
  /*point 6*/
  Brigade getCertainBrigade(int n) { return brigade.at(n); }
  vector<Brigade> getBrigades() { return brigade; }

  planOfWork getPlan() { return plan; }

  void printRequests(Householder hh) { hh.display(); }
  void displayBrig() {
    vector<Brigade>::iterator iter;
    int i;
    for (iter = brigade.begin(), i = 0; iter != brigade.end(); ++iter, i++) {
      cout << "\nBrigade №" << getCertainBrigade(i).id
           << "  Status: " << getCertainBrigade(i).status;
    }
  }
  void displayPlan() {
    cout << "\nCurrent plan of work: \n";
    cout << "Request: \n"
         << "Id: " << plan.request.getId()
         << "\nWork: " << plan.request.getKindWork()
         << "\nSize: " << plan.request.getSize()
         << "\nTime: " << plan.request.getTime() << "\nBrigade №"
         << plan.brigade_p.id + 1 << "\nHouseholder name: " << plan.name
         << endl;
  }
  void displayReq() {
    cout << "Id: " << plan.request.getId()
         << "\nWork: " << plan.request.getKindWork()
         << "\nSize: " << plan.request.getSize()
         << "\nTime: " << plan.request.getTime();
  }
  void deleteSomeBrigade(int n) { brigade.erase(brigade.begin() + n); }
  void deleteBrigades() { brigade.clear(); }
  void deleteRequest(Householder &hh, int n) { hh.deleteRequest(n); }
  bool checkBrigadesStatus() {
    for (auto item : brigade) {
      if (item.status == 1) {
        return 1;
      }
    }
    return 0;
  }
};

class PublicUtility {
private:
  /*point 15*/
  Householder *hholder;
  Request *request;
  Dispetcher *dispetcher;
  string name;

public:
  /*point 15*/
  PublicUtility(string name_p) {
    /*point 14, явний*/
    /*point 20*/
    hholder = new Householder();
    request = new Request();
    dispetcher = new Dispetcher();
    name = name_p;
  }

  string getName() { return name; }
  void getName(const char *filename) {
    ifstream stream_name(filename);
    stream_name.read((char *)&name, sizeof(PublicUtility));
  }

  /*getters*/
  /*point 13*/
  Householder getHouseholder() { return *hholder; }
  Request getRequest() { return *request; }
  Dispetcher getDispetcher() { return *dispetcher; }

  /*point 9*/
  ~PublicUtility() { cout << "\ndtor for " << name; }
};
