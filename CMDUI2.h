#include "faust/gui/console.h"

class CMDUI2 : public CMDUI
{
    private:
        std::thread* fThread;
        std::atomic<bool> fRun;
        
    public:
        CMDUI2(int argc, char* argv[], bool ignore_param = false)
        : CMDUI(argc, argv, ignore_param)
        {}
            
        static void update_gui2(CMDUI2* ui)
        {
            while (ui->fRun) {
                usleep(40 * 1000); // 25Hz
                GUI::updateAllGuis();
            }
        }
        
        bool run(std::function<bool(std::string)> handler)
        {
            fThread = new std::thread(update_gui2, this);
            std::cout << "Type 'q' to quit\n";
            std::string cmd;
            while (std::getline(std::cin, cmd)) {
                if (cmd.size() == 1 && cmd[0] == 'q') {
                    break;
                } else if (!handler(cmd)) {
                    break;
                }
                usleep(100 * 1000); 
            }
            fRun = false;
            fThread->join();
            return true;
        }
};