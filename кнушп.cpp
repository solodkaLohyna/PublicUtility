#include "sdfdsf.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
// #define brigFile "brigades.dat"
// #define planBrig "planBrigade.dat"
using namespace std;

struct emptynessOfFiles {
  bool emptyRequests;
  bool emptyBrigades;
  bool emptyPlanBrig;
  bool emptyPlanReq;
};

void dispetcherMenu(Dispetcher &disp, Householder &hh, Request &req,
                    emptynessOfFiles &empty);
/*point 11, 12*/
void count(Request req) {
  ifstream count_req("count.txt");
  int c;
  count_req.read((char *)&c, sizeof(Request));
  int k = req.getCount();
  cout << "\nNow, there are " << k << " request(-s)\n";
  ofstream set_req("count.txt", ios::trunc);
  set_req << k;
  set_req.close();
}

/*point 21*/
void count(Request *obj) {
  ifstream get_req("count.txt", ios::in);
  int k;
  get_req >> k;
  get_req.close();
  cout << k;
  obj->setCount(k);
  ofstream count_req("count.txt");
  int c = obj->getCount();
  count_req.write((char *)&c, sizeof(Request));
  cout << "There are " << obj->getCount() << " request(-s)\n\n";
}

void requestsToFile(Householder &hh, int id, string work, string size,
                    float time) {
  ofstream out(requestFile, ios::binary | ios::app);
  hh.writeInt(id, out);
  hh.write(work, out);
  hh.write(size, out);
  hh.writeFl(time, out);
  out.close();
}

void writeName(Householder &hh, string name) {
  ofstream streamName(householderName, ios::binary);
  hh.write(name, streamName);
  streamName.close();
}

void readName(Householder &hh) {
  ifstream streamName(householderName, ios::binary);
  hh.read(streamName);
  streamName.close();
}

void householder(Householder &hh, Request &req) {
  cout << "Filling information about request. . . \n";
  int num;
  cout << "Enter householder name: ";
  string name;
  cin >> name;
  hh.setName(name);
  writeName(hh, name);
  cout << "Enter number of requests: ";
  cin >> num;
  req.setCount(num);

  string work, size;
  float time;
  for (int i = 0; i < num; i++) {
    cout << "\nRequest №" << i + 1 << endl;
    cout << "Type of work: ";
    cin >> work;
    cout << "The sizes: ";
    cin >> size;
    cout << "Time: ";
    cin >> time;
    req.setInform(i + 1, work, size, time);
    requestsToFile(hh, i + 1, work, size, time);
    req.setHouseholderName(name);
    hh.setReq(req);
  }

  cout << "\nInformation was filed successfully. . .\n";
  cout << "Display information: \n";
  hh.display();
}

void read(Householder &hh, Request &req) {
  int number;
  ifstream f("count.txt");
  f >> number;
  f.close();
  ifstream name("householderName.bin", ios::binary);
  string hhName = hh.read(name);
  req.setHouseholderName(hhName);
  name.close();
  int id;
  string work, size;
  float time;
  ifstream in(requestFile, ios::binary|ios::app);
  for (int i = 0; i < number; i++) {
    id = hh.readInt(in);
    cout << id;
    work = hh.read(in);
    cout << work;
    size = hh.read(in);
    cout << size;
    time = hh.readFl(in);
    cout << time;
    req.setInform(id, work, size, time);
    hh.setReq(req);
  }

  in.close();
  hh.display();
}

bool isEmpty(string filename) {
  ofstream openToCreate(filename, ios::out | ios::app);
  openToCreate.close();

  ifstream isEmpty(filename, ios::ate | ios::binary);
  size_t size = isEmpty.tellg();
  cout << size;
  isEmpty.close();

  if (size <= 0)
    return true;

  return false;
}

void requestsManagment(Dispetcher &disp, Householder &hh, Request &req,
                       emptynessOfFiles &empty) {
                        cout << empty.emptyRequests;
  if (empty.emptyRequests == true && hh.getReq().empty() == true ||
      hh.getReq().empty() == true) {
    cout << "\nThere is no request. Return back and create at least 1 "
            "request!\n";
    householder(hh, req);
  }
  cout << "\nLets see requests. . .\n";
  hh.display();
  if (disp.checkBrigadesStatus() == 0) {
    int n;
    cout << "\nAll brigades are busy. . . \n";
  }
  int opt;
  cout << "Choose option: \n1 - To delete some (or all) request \n2 - Exit : ";
  cin >> opt;
  switch (opt) {
  case 1:
    cout << "You can delete requests before choosing '0' :";
    int n;
    cin >> n;
    if (n == 0) {
      cout << "Exit back. . .\n";
      requestsManagment(disp, hh, req, empty);
    }
    for (; n != 0;) {
      disp.deleteRequest(hh, n - 1);
      cout << "Deletion a " << n
           << " request successful!\nGo on or stop(enter '0'): ";
      cin >> n;
      if (n == 0) {
        cout << "Exit back. . .\n";
        requestsManagment(disp, hh, req, empty);
      }
    }
    break;

  case 2:
    dispetcherMenu(disp, hh, req, empty);
  }
}

void brigadesManagment(Dispetcher &disp, Householder &hh, Request &req,
                       emptynessOfFiles &empty) {
  cout << "\nIt's brigades managment\n";

  int opt;
  cout << "Choose option: \nTo check brigades - 1 \nTo change brigade status - "
          "2 \nTo create brigade - 3 \nTo delete brigade - 4 \nTo delete all "
          "brigade - 5 \nExit - 6: ";
  cin >> opt;
  switch (opt) {
    while (opt != 6) {
    case 1:
      if (empty.emptyBrigades == true && disp.getBrigades().empty() == true ||
          disp.getBrigades().empty() == true) {
        cout << "\nThere is no brigade\n";
        cout << "Please, return back and create at least 1 brigade!\n";
        brigadesManagment(disp, hh, req, empty);
      } else {
        cout << "\nLet's see list of brigades: \n";

        disp.displayBrig();
        cout << endl;
      }
      if (disp.checkBrigadesStatus() == 0) {
        int n;
        cout << "All brigades are busy. . . \n";
      }
      break;

    case 2:
      if (empty.emptyBrigades == true && disp.getBrigades().empty() == true) {
        cout << "\nThere is no brigade\n";
        cout << "Please, return back and create at least 1 brigade!\n";
        brigadesManagment(disp, hh, req, empty);
      } else {
        bool newStatus;
        int brigNum;
        cout << "\nChoose a brigade: ";
        cin >> brigNum;
        cout << "Enter status: ";
        cin >> newStatus;
        disp.changeStatus(brigNum - 1, newStatus);
      }
      break;

    case 3: {
      cout << "\nCreating brigade. . .\n";
      bool status;
      int id, n;
      cout << "How much brigades you need to create? ";
      cin >> n;

      if (disp.getBrigades().empty() == true) {
        for (int i = 0; i < n; i++) {
          id = i + 1;
          cout << "Brigade № " << id << " with status: true\n";
          status = 1;
          disp.setBrig(status, id);
        }
        cout << endl;
      } else {
        for (int i = 0, k = 1; i < n; i++, k++) {
          // for (int l = 0; l < disp.getBrigades().size(); l++) {
          //   if (k == disp.getCertainBrigade(l).id) {
          //     k++;
          //   } else {
          id = k;
          cout << "Brigade № " << id << " with status: true\n";
          status = 1;
          disp.setBrig(status, id);
        }
        // }
      }

      ofstream saveBrig(brigFile, ios::binary | ios::out | ios::app);
      disp.save(saveBrig);
      saveBrig.close();

      break;
    }

    case 4:
      if (empty.emptyBrigades == true && disp.getBrigades().empty() == true) {
        cout << "\nThere is no brigade\n";
        cout << "Please, return back and create at least 1 brigade!\n";
        brigadesManagment(disp, hh, req, empty);
      } else {
        int numberBrigade;
        cout << "\nChoose a brigade to delete: ";
        cin >> numberBrigade;
        disp.deleteSomeBrigade(numberBrigade - 1);

        ofstream truncBrigade(brigFile, ios::binary | ios::out);
        disp.save(truncBrigade);
        truncBrigade.close();

        cout << "Deletion successful!\n";
      }
      break;

    case 5:
      if (disp.getBrigades().empty() == true)
        cout << "\nThere is no brigade!/n";
      else {
        cout << "\nDeleting all brigades. . .\n";
        disp.deleteBrigades();
        disp.deleteFile(brigFile);
        cout << "Deleting successful!\n";
      }
      break;
    }
  case 6:
    dispetcherMenu(disp, hh, req, empty);
    break;
  }
}

void setPlanOfWork(Dispetcher &disp, Householder &hh, Request &req,
                   emptynessOfFiles &empty) {
  if (empty.emptyPlanBrig != true) {
    if (empty.emptyPlanReq != true) {
    }
    if (disp.getBrigades().empty() == true) {
      cout << "\nNo one brigade isn't aviable/created!\n";
      dispetcherMenu(disp, hh, req, empty);
    }

    if (hh.getReq().empty() == true) {
      cout << "\nNo one request isn't aviable/created!\n";
      dispetcherMenu(disp, hh, req, empty);
    }

    cout << "Let's create plan of work: \n";
    int n, k;
    cout << "Choose a brigade: ";
    cin >> k;
    disp.setBrigadeToPlan(k - 1);
    cout << "Choose a request: ";
    cin >> n;
    disp.setRequestToPlan(n - 1, hh);

    cout << "Plan of work was filed successfully\n";
    cout << "\nDisplaying information. . .\n";
    disp.displayPlan();
  } else {
    cout << "\nThere is some information\nDisplaying. . .";
    disp.displayPlan();
  }
}

void dispetcherMenu(Dispetcher &disp, Householder &hh, Request &req,
                    emptynessOfFiles &empty) {
  int opt;
  cout << "\nPlease, choose one option: \n";
  cout << "Go to requests managment - 1 \nGo to brigades managment - 2 \nGo "
          "to "
          "plan of work - 3 \nExit - 0 : ";
  cin >> opt;
  switch (opt) {
  case 1:
    requestsManagment(disp, hh, req, empty);
    break;

  case 2:
    brigadesManagment(disp, hh, req, empty);
    break;

  case 3:
    setPlanOfWork(disp, hh, req, empty);
    break;

  case 0:
    return;
    break;
  }
}

int main() {
  /*point 14, простий*/
  PublicUtility pub("Sonechko");
  emptynessOfFiles statusContainer;
  /*point 10*/
  Householder hh = pub.getHouseholder();
  Request req = pub.getRequest();
  Dispetcher disp = pub.getDispetcher();
  count(&req);
  statusContainer.emptyRequests = isEmpty(requestFile);
  cout <<(string*)statusContainer.emptyRequests;

  if (statusContainer.emptyRequests != true)
    read(hh, req);

  // hh.display();

  cout << "\nIt's requests managment";
  requestsManagment(disp, hh, req, statusContainer);
  // householder(hh, req);
  count(req);

  cout << "\nHi there, dispetcher!";

  statusContainer.emptyBrigades = isEmpty(brigFile);
  statusContainer.emptyPlanBrig = isEmpty(planBrig);
  statusContainer.emptyPlanReq = isEmpty(planReq);

  if (statusContainer.emptyBrigades != true)
    disp.read();
  if (statusContainer.emptyPlanBrig != true)
    disp.readPlanBrigade();
  if (statusContainer.emptyPlanReq != true)
    disp.readPlaneRequest();
  // dispetcherMenu(disp, hh, req, statusContainer);

  // int opt;
  // cout
  //     << "Choose a way to enter: 1 - householder, 2 - dispetcher, 0 -
  //     return:
  //     ";
  // cin >> opt;
  // switch (opt) {
  // case 0:
  //   return 0;
  //   break;

  // case 1:
  // householder(hh, req);
  //   count(req);
  //   break;

  // case 2:
  //   /*point 10*/
  //   Dispetcher disp = pub.getDispetcher();
  //   dispetcherMenu(disp, hh);
  //   break;
  // }

  return 0;
}