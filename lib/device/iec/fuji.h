#ifndef FUJI_H
#define FUJI_H

#include <cstdint>
#include <cstring>

#include "bus.h"
#include "network.h"
#include "cassette.h"

#include "../fuji/fujiHost.h"
#include "../fuji/fujiDisk.h"
#include "../fuji/fujiCmd.h"

#define MAX_HOSTS 8
#define MAX_DISK_DEVICES 8
#define MAX_NETWORK_DEVICES 4

#define MAX_APPKEY_LEN 64

typedef struct
{
    char ssid[33];
    char password[64];
    char hostname[64];
    unsigned char localIP[4];
    unsigned char gateway[4];
    unsigned char netmask[4];
    unsigned char dnsIP[4];
    unsigned char macAddress[6];
    unsigned char bssid[6];
    char fn_version[15];
} AdapterConfig;

enum appkey_mode : int8_t
{
    APPKEYMODE_INVALID = -1,
    APPKEYMODE_READ = 0,
    APPKEYMODE_WRITE,
    APPKEYMODE_READ_256
};

struct appkey
{
    uint16_t creator = 0;
    uint8_t app = 0;
    uint8_t key = 0;
    appkey_mode mode = APPKEYMODE_INVALID;
    uint8_t reserved = 0;
} __attribute__((packed));


class iecFuji : public virtualDevice
{
private:
    systemBus *_bus;

    fujiHost _fnHosts[MAX_HOSTS];

    fujiDisk _fnDisks[MAX_DISK_DEVICES];

    int _current_open_directory_slot = -1;

    iecDrive _bootDisk; // special disk drive just for configuration

    uint8_t bootMode = 0; // Boot mode 0 = CONFIG, 1 = MINI-BOOT

    uint8_t _countScannedSSIDs = 0;

    appkey _current_appkey;

    AdapterConfig cfg;

    std::string response;
    std::vector<uint8_t> responseV;
    bool is_raw_command;

    void process_raw_commands();
    void process_basic_commands();
    vector<string> tokenize_basic_command(string command);

    bool validate_parameters_and_setup(uint8_t& maxlen, uint8_t& addtlopts);
    bool validate_directory_slot();
    std::string process_directory_entry(uint8_t maxlen, uint8_t addtlopts);
    void format_and_set_response(const std::string& entry);

protected:
    void reset_device();           // 0xFF

    void net_get_ssid();           // 0xFE
    void net_scan_networks();      // 0xFD
    void net_scan_result(int n);   // 0xFC
    void net_scan_result_basic();
    void net_scan_result_raw();
    void net_set_ssid( bool store = true );           // 0xFB
    void net_store_ssid(std::string ssid, std::string password);
    void net_get_wifi_status();    // 0xFA
    
    void mount_host();             // 0xF9

    bool disk_image_mount(uint8_t ds, uint8_t mode); // 0xF8
    void disk_image_mount_raw();
    void disk_image_mount_basic();

    bool open_directory(uint8_t hs, std::string dirpath, std::string pattern); // 0xF7
    void open_directory_basic();
    void open_directory_raw();

    void read_directory_entry();   // 0xF6
    void close_directory();        // 0xF5
    void read_host_slots();        // 0xF4
    void write_host_slots();       // 0xF3
    void read_device_slots();      // 0xF2
    void write_device_slots();     // 0xF1
    
    void enable_udpstream();       // 0xF0
    void net_get_wifi_enabled();   // 0xEA
    
    void disk_image_umount();      // 0xE9
    
    void get_adapter_config();     // 0xE8
    
    void new_disk();               // 0xE7
    void unmount_host();           // 0xE6

    void get_directory_position(); // 0xE5
    void set_directory_position(); // 0xE4

    void set_hindex();             // 0xE3
    void set_device_filename();    // 0xE2
    void set_host_prefix();        // 0xE1
    void get_host_prefix();        // 0xE0
    
    void set_external_clock();     // 0xDF
    
    void write_app_key();          // 0xDE
    void read_app_key();           // 0xDD
    void open_app_key();           // 0xDC
    void close_app_key();          // 0xDB
    
    void get_device_filename();    // 0xDA
    void set_boot_config();        // 0xD9
    void copy_file();              // 0xD8
    void set_boot_mode();          // 0xD6

    // Commodore specific
    void local_ip();

    device_state_t process() override;

    void shutdown() override;

protected:

    /**
     * @brief called to process command either at open or listen
     */
    void iec_command();

public:
    bool boot_config = true;

    bool status_wait_enabled = true;
    
    //iecNetwork *network();

    iecDrive *bootdisk();

    void insert_boot_device(uint8_t d);

    void setup(systemBus *bus);

    void image_rotate();
    int get_disk_id(int drive_slot);
    std::string get_host_prefix(int host_slot);

    fujiHost *get_hosts(int i) { return &_fnHosts[i]; }
    fujiDisk *get_disks(int i) { return &_fnDisks[i]; }

    void _populate_slots_from_config();
    void _populate_config_from_slots();

    void mount_all();              // 0xD7

    iecFuji();
};

extern iecFuji theFuji;

#endif // FUJI_H