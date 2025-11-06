#!/usr/bin/env python3
"""
World Generator Script for Dynamic Pedestrian Scenarios

This script generates Gazebo SDF world files from Jinja2 templates and YAML configurations.
It allows easy modification of pedestrian count, positions, and trajectories without
manually editing large XML files.

Usage:
    python3 generate_world.py --template <template.sdf.jinja2> --config <config.yaml> --output <output.sdf>
"""

import argparse
import sys
from pathlib import Path

try:
    import yaml
    from jinja2 import Environment, FileSystemLoader, TemplateNotFound
except ImportError as e:
    print(f"ERROR: Missing required Python package: {e}", file=sys.stderr)
    print("Please install: pip3 install pyyaml jinja2", file=sys.stderr)
    sys.exit(1)


def load_yaml_config(config_path: Path) -> dict:
    """
    Load pedestrian configuration from YAML file.

    Args:
        config_path: Path to YAML configuration file

    Returns:
        Dictionary containing pedestrian configurations

    Raises:
        FileNotFoundError: If config file doesn't exist
        yaml.YAMLError: If YAML parsing fails
    """
    if not config_path.exists():
        raise FileNotFoundError(f"Configuration file not found: {config_path}")

    with open(config_path, 'r') as f:
        config = yaml.safe_load(f)

    if not config or 'pedestrians' not in config:
        raise ValueError("Config file must contain 'pedestrians' key")

    return config


def validate_pedestrian_config(config: dict) -> None:
    """
    Validate pedestrian configuration structure.

    Args:
        config: Configuration dictionary

    Raises:
        ValueError: If configuration is invalid
    """
    pedestrians = config.get('pedestrians', [])

    if not isinstance(pedestrians, list):
        raise ValueError("'pedestrians' must be a list")

    if len(pedestrians) == 0:
        print("WARNING: No pedestrians defined in configuration", file=sys.stderr)
        return

    required_fields = ['name', 'spawn_pose', 'trajectory']

    for idx, ped in enumerate(pedestrians):
        # Check required fields
        for field in required_fields:
            if field not in ped:
                raise ValueError(f"Pedestrian {idx} missing required field: {field}")

        # Validate spawn_pose
        if not isinstance(ped['spawn_pose'], list) or len(ped['spawn_pose']) != 6:
            raise ValueError(f"Pedestrian '{ped['name']}': spawn_pose must be list of 6 numbers [x,y,z,roll,pitch,yaw]")

        # Validate trajectory
        trajectory = ped['trajectory']
        if not isinstance(trajectory, list) or len(trajectory) < 2:
            raise ValueError(f"Pedestrian '{ped['name']}': trajectory must have at least 2 waypoints")

        for wp_idx, waypoint in enumerate(trajectory):
            if 'time' not in waypoint or 'pose' not in waypoint:
                raise ValueError(f"Pedestrian '{ped['name']}' waypoint {wp_idx}: missing 'time' or 'pose'")

            if not isinstance(waypoint['pose'], list) or len(waypoint['pose']) != 6:
                raise ValueError(f"Pedestrian '{ped['name']}' waypoint {wp_idx}: pose must be list of 6 numbers")


def generate_world(template_path: Path, config: dict, output_path: Path) -> None:
    """
    Generate world SDF file from template and configuration.

    Args:
        template_path: Path to Jinja2 template file
        config: Pedestrian configuration dictionary
        output_path: Path to output SDF file

    Raises:
        TemplateNotFound: If template file doesn't exist
    """
    # Setup Jinja2 environment
    template_dir = template_path.parent
    template_name = template_path.name

    env = Environment(
        loader=FileSystemLoader(template_dir),
        trim_blocks=True,
        lstrip_blocks=True
    )

    # Load template
    try:
        template = env.get_template(template_name)
    except TemplateNotFound:
        raise FileNotFoundError(f"Template not found: {template_path}")

    # Render template
    rendered = template.render(pedestrians=config['pedestrians'])

    # Write output
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, 'w') as f:
        f.write(rendered)

    print(f"✓ Generated world file: {output_path}")
    print(f"  - {len(config['pedestrians'])} pedestrian(s) configured")


def main():
    """Main entry point for world generator."""
    parser = argparse.ArgumentParser(
        description="Generate Gazebo world files with dynamic pedestrian actors",
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        '--template',
        type=Path,
        required=True,
        help='Path to Jinja2 template file (*.sdf.jinja2)'
    )

    parser.add_argument(
        '--config',
        type=Path,
        required=True,
        help='Path to pedestrian configuration YAML file'
    )

    parser.add_argument(
        '--output',
        type=Path,
        required=True,
        help='Path to output SDF world file'
    )

    args = parser.parse_args()

    try:
        # Load and validate configuration
        print(f"Loading configuration: {args.config}")
        config = load_yaml_config(args.config)

        print("Validating configuration...")
        validate_pedestrian_config(config)

        # Generate world file
        print(f"Rendering template: {args.template}")
        generate_world(args.template, config, args.output)

        print("✓ World generation completed successfully")
        return 0

    except FileNotFoundError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return 1
    except (ValueError, yaml.YAMLError) as e:
        print(f"ERROR: Invalid configuration: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"ERROR: Unexpected error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
