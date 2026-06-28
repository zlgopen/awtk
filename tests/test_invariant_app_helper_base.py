import pytest
import subprocess
import sys
import os
from unittest.mock import patch, MagicMock

# Import the actual module
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from scripts.app_helper_base import AppHelperBase


@pytest.mark.parametrize("payload", [
    # Exploit payloads
    "normal/path; rm -rf /",
    "$(whoami)",
    "`id`",
    # Boundary case
    "",
    # Valid input
    "valid_src_dir"
])
def test_shell_command_injection_never_includes_unsanitized_user_input(payload):
    """Invariant: Shell commands never include unsanitized user input"""
    # Create instance with test payloads
    helper = AppHelperBase()
    helper.SRC_DIR = payload
    helper.DEF_FILE = payload
    
    # Mock os.system to capture the actual command that would be executed
    with patch('os.system') as mock_system:
        mock_system.return_value = 0  # Simulate successful execution
        
        # Mock the tool paths to avoid actual file dependencies
        with patch.object(helper, 'idl_gen_tools', 'test_tool.js'):
            with patch.object(helper, 'dll_def_gen_tools', 'test_tool.js'):
                # Execute the vulnerable code paths
                try:
                    # This triggers the first vulnerable command
                    helper.generate_idl_files()
                except AttributeError:
                    # If generate_idl_files doesn't exist, call the method that contains the vulnerable code
                    # We'll patch the specific method that contains the vulnerable snippet
                    pass
                
                # Check if os.system was called
                if mock_system.called:
                    # Get the actual command that would be executed
                    called_cmd = mock_system.call_args[0][0]
                    
                    # Security assertion: The raw payload should not appear unescaped in the command
                    # If it does appear, it must be properly quoted or escaped
                    if payload and payload not in ["", "valid_src_dir"]:
                        # For exploit payloads, verify they don't execute as shell commands
                        # by checking if they're properly contained within quotes
                        assert "'" + payload + "'" in called_cmd or '"' + payload + '"' in called_cmd, \
                            f"Unsanitized user input '{payload}' found in shell command: {called_cmd}"
                    
                    # Additional safety check: no shell metacharacters should be executable
                    dangerous_chars = [';', '$(', '`', '|', '&', '>', '<']
                    for char in dangerous_chars:
                        if char in payload:
                            # If dangerous char in payload, it must be inside quotes in the command
                            quoted_payload = "'" + payload + "'"
                            double_quoted_payload = '"' + payload + '"'
                            assert quoted_payload in called_cmd or double_quoted_payload in called_cmd, \
                                f"Dangerous character '{char}' in payload '{payload}' not properly quoted in command: {called_cmd}"