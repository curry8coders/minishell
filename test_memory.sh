#!/bin/bash

# Test memory cleanup with leaks command (macOS)
echo "Testing memory cleanup..."
echo ""

# Test 1: Exit command
echo "=== Test 1: Exit command ==="
echo -e "export TEST=hello\necho \$TEST\nexit 0" | ./minishell &
PID=$!
sleep 1
echo "Running leaks on PID $PID..."
leaks $PID 2>&1 | grep -E "(LEAK|leaked bytes)" || echo "Process already exited (expected)"
wait $PID
echo ""

# Test 2: Ctrl+D (EOF)
echo "=== Test 2: Ctrl+D (EOF) ==="
echo -e "export VAR1=test1\nexport VAR2=test2\necho done" | ./minishell &
PID=$!
sleep 1
echo "Running leaks on PID $PID..."
leaks $PID 2>&1 | grep -E "(LEAK|leaked bytes)" || echo "Process already exited (expected)"
wait $PID
echo ""

echo "Memory tests completed!"

