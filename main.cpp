#include "c_core.h"
#include "devices.h"

//
// A --|>o -- !A
//
class NAND_NOT : public Device
{
public:
    NAND_NOT(Device* parent_device_pointer, std::string name, bool monitor_on, std::vector<state_descriptor> input_default_states)
    : Device(parent_device_pointer, name, "nand_not", {"In"}, {"Out"}, monitor_on, input_default_states)
    {
        Build();
        Stabilise();
    }

    void Build()
    {
        AddGate("nand_0", "nand", {"input_0", "input_1"}, false);

        Connect("In", "nand_0", "input_0");
        Connect("In", "nand_0", "input_1");

        ChildConnect("nand_0", {"parent", "Out"});
    }
};

//
// A --|&&
//     |&&-- A*B (A AND B)
// B --|&&
//
class NAND_AND : public Device
{
public:
    NAND_AND(Device* parent_device_pointer, std::string name, bool monitor_on, std::vector<state_descriptor> input_default_states)
    : Device(parent_device_pointer, name, "nand_and", {"In_0", "In_1"}, {"Out"}, monitor_on, input_default_states)
    {
        Build();
        Stabilise();
    }

    void Build()
    {
        AddGate("nand_0", "nand", {"input_0", "input_1"}, false);
        AddComponent(new NAND_NOT(this, "not_0", false, {{"In", false}}));

        Connect("In_0", "nand_0", "input_0");
        Connect("In_1", "nand_0", "input_1");

        ChildConnect("nand_0", {"not_0", "In"});

        ChildConnect("not_0", {"Out", "parent", "Out"});
    }
};

//
// A --|OR
//     |OR-- A+B (A OR B)
// B --|OR
//
class NAND_OR : public Device
{
public:
    NAND_OR(Device* parent_device_pointer, std::string name, bool monitor_on, std::vector<state_descriptor> input_default_states)
    : Device(parent_device_pointer, name, "nand_or", {"In_0", "In_1"}, {"Out"}, monitor_on, input_default_states)
    {
        Build();
        Stabilise();
    }

    void Build()
    {
        AddComponent(new NAND_NOT(this, "not_0", false, {{"In", false}}));
        AddComponent(new NAND_NOT(this, "not_1", false, {{"In", false}}));
        AddGate("nand_0", "nand", {"input_0", "input_1"}, false);

        Connect("In_0", "not_0", "In");
        Connect("In_1", "not_1", "In");

        ChildConnect("not_0", {"Out", "nand_0", "input_0"});
        ChildConnect("not_1", {"Out", "nand_0", "input_1"});

        ChildConnect("nand_0", {"parent", "Out"});
    }
};

//
// A --|XR
//     |XR-- A XOR B
// B --|XR
//
class NAND_XOR : public Device
{
public:
    NAND_XOR(Device* parent_device_pointer, std::string name, bool monitor_on, std::vector<state_descriptor> input_default_states)
    : Device(parent_device_pointer, name, "nand_or", {"In_0", "In_1"}, {"Out"}, monitor_on, input_default_states)
    {
        Build();
        Stabilise();
    }

    void Build()
    {
        AddGate("nand_0", "nand", {"input_0", "input_1"});
        AddGate("nand_1", "nand", {"input_0", "input_1"});
        AddGate("nand_2", "nand", {"input_0", "input_1"});
        AddGate("nand_3", "nand", {"input_0", "input_1"});

        Connect("In_0", "nand_0", "input_0");
        Connect("In_0", "nand_1", "input_0");

        Connect("In_1", "nand_0", "input_1");
        Connect("In_1", "nand_2", "input_1");

        ChildConnect("nand_0", {"nand_1", "input_1"});
        ChildConnect("nand_0", {"nand_2", "input_0"});

        ChildConnect("nand_1", {"nand_3", "input_0"});
        ChildConnect("nand_2", {"nand_3", "input_1"});

        ChildConnect("nand_3", {"parent", "Out"});
    }
};

void test_not()
{
    bool verbose = false;
    bool monitor_on = false;
    bool print_probe_samples = true;

    Simulation sim("test_sim", verbose);

    sim.AddComponent(new NAND_NOT(&sim, "test_not", monitor_on, {{"In", false}}));

    sim.Stabilise();

    sim.AddClock("clock_0", {false, true}, monitor_on);
    sim.ClockConnect("clock_0", "test_not", "In");

    sim.AddProbe("output_0", "test_sim:test_not", {"In", "Out"}, "clock_0");

    sim.Run(4, false, verbose, print_probe_samples);
}

void test_and()
{
    bool verbose = false;
    bool monitor_on = false;
    bool print_probe_samples = true;

    Simulation sim("test_sim", verbose);

    sim.AddComponent(new NAND_AND(&sim, "test_and", monitor_on, {{"In_0", false}, {"In_1", false}}));

    sim.Stabilise();

    sim.AddClock("clock_0", {false, true}, monitor_on);
    sim.AddClock("clock_1", {false, false, true, true}, monitor_on);
    sim.ClockConnect("clock_0", "test_and", "In_0");
    sim.ClockConnect("clock_1", "test_and", "In_1");

    sim.AddProbe("output_0", "test_sim:test_and", {"In_0", "In_1", "Out"}, "clock_0");

    sim.Run(4, false, verbose, print_probe_samples);

}

void test_or()
{
    bool verbose = false;
    bool monitor_on = false;
    bool print_probe_samples = true;

    Simulation sim("test_sim", verbose);

    sim.AddComponent(new NAND_OR(&sim, "test_or", monitor_on, {{"In_0", false}, {"In_1", false}}));

    sim.Stabilise();

    sim.AddClock("clock_0", {false, true}, monitor_on);
    sim.AddClock("clock_1", {false, false, true, true}, monitor_on);
    sim.ClockConnect("clock_0", "test_or", "In_0");
    sim.ClockConnect("clock_1", "test_or", "In_1");

    sim.AddProbe("output_0", "test_sim:test_or", {"In_0", "In_1", "Out"}, "clock_0");

    sim.Run(4, false, verbose, print_probe_samples);

}

void test_xor()
{
    bool verbose = false;
    bool monitor_on = false;
    bool print_probe_samples = true;

    Simulation sim("test_sim", verbose);

    sim.AddComponent(new NAND_XOR(&sim, "test_xor", monitor_on, {{"In_0", false}, {"In_1", false}}));

    sim.Stabilise();

    sim.AddClock("clock_0", {false, true}, monitor_on);
    sim.AddClock("clock_1", {false, false, true, true}, monitor_on);
    sim.ClockConnect("clock_0", "test_xor", "In_0");
    sim.ClockConnect("clock_1", "test_xor", "In_1");

    sim.AddProbe("output_0", "test_sim:test_xor", {"In_0", "In_1", "Out"}, "clock_0");

    sim.Run(4, false, verbose, print_probe_samples);

}

int main ()
{
    test_not();
    test_and();
    test_or();
    test_xor();

    return 0;
}
