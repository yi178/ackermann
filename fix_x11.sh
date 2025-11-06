#!/bin/bash

echo "=========================================="
echo "  X11 问题诊断与修复"
echo "=========================================="
echo ""

# 1. 检查DISPLAY变量
echo "1. 检查DISPLAY变量..."
if [ -z "$DISPLAY" ]; then
    echo "   ❌ DISPLAY未设置"
    echo "   尝试设置为 :0"
    export DISPLAY=:0
else
    echo "   ✓ DISPLAY = $DISPLAY"
fi

# 2. 清理X11锁文件
echo ""
echo "2. 清理X11锁文件..."
sudo rm -f /tmp/.X*-lock 2>/dev/null
sudo rm -rf /tmp/.X11-unix 2>/dev/null
sudo mkdir -p /tmp/.X11-unix
sudo chmod 1777 /tmp/.X11-unix
echo "   ✓ 清理完成"

# 3. 测试X11连接
echo ""
echo "3. 测试X11连接..."
if command -v xdpyinfo &> /dev/null; then
    if xdpyinfo &> /dev/null; then
        echo "   ✓ X11连接正常"
    else
        echo "   ❌ X11连接失败"
        echo "   建议：重新建立SSH连接（启用X11转发）"
    fi
else
    echo "   ⚠ xdpyinfo未安装，跳过测试"
fi

# 4. 检查Gazebo进程
echo ""
echo "4. 检查残留进程..."
if pgrep -x gz > /dev/null; then
    echo "   ⚠ 发现Gazebo进程，清理中..."
    pkill -9 gz
    sleep 1
    echo "   ✓ 已清理"
else
    echo "   ✓ 无残留进程"
fi

if pgrep -x rviz2 > /dev/null; then
    echo "   ⚠ 发现RViz进程，清理中..."
    pkill -9 rviz2
    sleep 1
    echo "   ✓ 已清理"
fi

echo ""
echo "=========================================="
echo "  修复完成！"
echo "=========================================="
echo ""
echo "解决方案选择："
echo ""
echo "方案A（推荐）：重新建立SSH连接"
echo "  1. 退出当前会话: exit"
echo "  2. 使用SSH重连: ssh -X yiyi@your-ip"
echo "  3. 验证X11: xeyes"
echo ""
echo "方案B：使用无头模式（不显示GUI）"
echo "  ros2 launch saye_bringup_my saye_spawn.launch.py \\"
echo "    world:=saye_moving_obstacles.sdf \\"
echo "    gui:=false"
echo ""
echo "方案C：在WSL2内使用WSLg（如果可用）"
echo "  export DISPLAY=:0"
echo "  然后正常启动"
echo ""
