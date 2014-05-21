#include "Kernel.h"
#include "PCSCConnector.h"

using namespace std;

int main(int argc, char* argv[]) {
   PCSCConnector* c = new PCSCConnector();
   c->initialize();
   
   Kernel k(c);
   vector<ApplicationInfo> aids = k.getCardAIDs();
}