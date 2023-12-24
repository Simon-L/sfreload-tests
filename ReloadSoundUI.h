// ReloadSoundUI keeps a map that associates labels used in dsp code
// with pointers to Soundfile zones so that they can be reloaded
// while the DSP is running

class ReloadSoundUI : public SoundUI
{   
    std::map<std::string, Soundfile**> fSfZoneMap;
    
    public:
        ReloadSoundUI(const std::string& sound_directory = "", int sample_rate = -1, SoundfileReader* reader = nullptr, bool is_double = false)
            : SoundUI(sound_directory, sample_rate, reader, is_double)
        {
            
        }

        ~ReloadSoundUI() {}
        
        virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
        {   
            const char* saved_url = url; // 'url' is consumed by parseMenuList2
            std::vector<std::string> file_name_list;            
            
            bool menu = parseMenuList2(url, file_name_list, true);
            // If not a list, we have as single file
            if (!menu) { file_name_list.push_back(saved_url); }
            
            // Parse the possible list
            std::string saved_url_real = std::string(saved_url) + "_" + std::to_string(fIsDouble); // fIsDouble is used in the key
            if (fSoundfileMap.find(saved_url_real) == fSoundfileMap.end()) {
                // Check all files and get their complete path
                std::vector<std::string> path_name_list = fSoundReader->checkFiles(fSoundfileDir, file_name_list);
                if (path_name_list.size() > 1) {
                    std::cerr << "addSoundfile : ReloadSoundUI only supports one file! " << path_name_list.size() << " files provided." << std::endl;
                    *sf_zone = defaultsound;
                    return;
                }
                // Read them and create the Soundfile
                Soundfile* sound_file = fSoundReader->createSoundfile(path_name_list, MAX_CHAN, fIsDouble);
                if (sound_file) {
                    fSoundfileMap[saved_url_real] = std::shared_ptr<Soundfile>(sound_file);
                } else {
                    // If failure, use 'defaultsound'
                    std::cerr << "addSoundfile : soundfile for " << saved_url << " cannot be created !" << std::endl;
                    *sf_zone = defaultsound;
                    return;
                }
            }
            
            // Keep a map of label to sf_zone to access it later
            fSfZoneMap[label] = sf_zone;
            // Get the soundfile pointer
            *sf_zone = fSoundfileMap[saved_url_real].get();
        }
        
        void printSoundfilesInfos()
        {
            for (const auto &item : fSfZoneMap) {
                auto sf = *(item.second);
                std::cout << item.first <<  ": Length: " << sf->fLength[0] << " SR: " << sf->fSR[0] << " Channels: " << sf->fChannels << '\n';
            }
        }
        
        Soundfile* createNewSoundfile(std::string path)
        {
            std::vector<std::string> path_name_list;
            path_name_list.push_back(path);
            path_name_list = fSoundReader->checkFiles(fSoundfileDir, path_name_list);
            if (path_name_list[0] == "__empty_sound__") {
                std::cerr << "createNewSoundfile: cannot find " << path << std::endl;
                return nullptr;
            }
            
            std::string saved_url_real = std::string(path) + "_" + std::to_string(fIsDouble); // fIsDouble is used in the key

            Soundfile* sound_file = fSoundReader->createSoundfile(path_name_list, MAX_CHAN, fIsDouble);
            if (sound_file) {
                fSoundfileMap[saved_url_real] = std::shared_ptr<Soundfile>(sound_file);
            } else {
                // If failure, use 'defaultsound'
                std::cerr << "createNewSoundfile : soundfile for " << path << " cannot be created !" << std::endl;
                return nullptr;
            }
            return sound_file;
        }
        
        Soundfile* updateSoundfile(std::string label, Soundfile* newSoundfile)
        {   
            if (fSfZoneMap.find(label) == fSfZoneMap.end()) {
                return nullptr;
            }
            auto oldSoundfile = fSfZoneMap[label];
            *(fSfZoneMap[label]) = newSoundfile;
            return *oldSoundfile;
        }
};