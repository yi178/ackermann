# MPPI RiskAwareCritic Configuration Fix

## Problem

The MPPI experiment launch was failing with this error:
```
[lifecycle_manager-6] [ERROR] Failed to change state for node: controller_server
[lifecycle_manager-6] [ERROR] Failed to bring up all requested nodes. Aborting bringup.
```

## Root Cause

The `mppi_nav2_params.yaml` configuration file had inconsistent RiskAwareCritic setup:

**Before (broken):**
```yaml
critics: [
  "ConstraintCritic", "CostCritic", "GoalCritic",
  "PathAlignCritic", "PathFollowCritic", "PreferForwardCritic",
  "saye_rapa_mppi::RiskAwareCritic"  # Fully-qualified name
]
# ...
RiskAwareCritic:  # Config block with simple name
  enabled: True
  cost_power: 1
  cost_weight: 1.0
  # MISSING: plugin field!
```

### Why This Failed

For **custom critics** (not built into Nav2), you must:
1. Use a **simple name** in the critics list (e.g., `"RiskAwareCritic"`)
2. Provide a **plugin field** in the config block to specify the full class name

The configuration had evolved through multiple commits:
- Initially had `plugin` field
- Was changed to use fully-qualified names everywhere, removing the `plugin` field
- Was changed back to simple name, but `plugin` field wasn't restored

## Solution

**After (fixed):**
```yaml
critics: [
  "ConstraintCritic", "CostCritic", "GoalCritic",
  "PathAlignCritic", "PathFollowCritic", "PreferForwardCritic",
  "RiskAwareCritic"  # Simple name
]
# ...
RiskAwareCritic:  # Config block with simple name
  plugin: "saye_rapa_mppi::RiskAwareCritic"  # Plugin field added!
  enabled: True
  cost_power: 1
  cost_weight: 1.0
```

## Changes Made

### 1. Updated `saye_mppi/config/mppi_nav2_params.yaml`
- Changed critics list from `"saye_rapa_mppi::RiskAwareCritic"` to `"RiskAwareCritic"`
- Added `plugin: "saye_rapa_mppi::RiskAwareCritic"` to the RiskAwareCritic config block

### 2. Created diagnostic script: `diagnose_mppi.sh`
Run this script to verify the plugin is properly configured:
```bash
./diagnose_mppi.sh
```

This checks:
- Plugin library exists
- Plugin registration (plugins.xml)
- Config file correctness
- Message definitions available
- Plugin discoverability via pluginlib

## Next Steps

### 1. Rebuild the workspace
```bash
colcon build --packages-select saye_mppi
source install/setup.bash
```

### 2. Run diagnostics (optional)
```bash
./diagnose_mppi.sh
```

### 3. Test the launch
```bash
# Terminal 1: Start simulation with dynamic corridor
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf

# Terminal 2: Start MPPI experiment
ros2 launch saye_mppi mppi_experiment.launch.py
```

You should see:
- Controller server successfully configures and activates
- RiskAwareCritic logs: `RiskAwareCritic initialized (Phase 1 - Framework Only)`
- No lifecycle manager errors

## Configuration Pattern Reference

For any **built-in Nav2 critic** (e.g., ConstraintCritic, CostCritic):
```yaml
critics: ["ConstraintCritic"]
ConstraintCritic:
  enabled: True
  cost_weight: 4.0
  # No plugin field needed
```

For any **custom critic** (plugin from external package):
```yaml
critics: ["MyCustomCritic"]  # Simple name
MyCustomCritic:
  plugin: "my_package::MyCustomCritic"  # Full class name
  enabled: True
  cost_weight: 1.0
  # Your custom parameters...
```

## Related Files

- Config: `src/ackermann-vehicle-gzsim-ros2/saye_mppi/config/mppi_nav2_params.yaml`
- Launch: `src/ackermann-vehicle-gzsim-ros2/saye_mppi/launch/mppi_experiment.launch.py`
- Plugin: `src/ackermann-vehicle-gzsim-ros2/saye_rapa_mppi/src/risk_aware_critic.cpp`
- Registration: `src/ackermann-vehicle-gzsim-ros2/saye_rapa_mppi/plugins.xml`

## Git Commit History

Recent commits that led to this issue:
1. `0a8fdce` - Added plugin field (correct)
2. `8e6d427` - Used fully-qualified names, removed plugin field (incorrect approach)
3. `7a61e95` - Reverted to simple name, forgot to restore plugin field (incomplete)
4. Latest commit - Restored plugin field (fixes issue)
