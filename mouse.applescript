-- Open System Preferences and navigate to Accessibility
tell application "System Preferences"
    activate
    set current pane to pane "com.apple.preference.universalaccess"
end tell

-- Change the Tracking Speed
tell application "System Events"
    tell process "System Preferences"
        set frontmost to true
        delay 1 -- You may need to adjust the delay based on your system's responsiveness
        tell table 1 of scroll area 1 of window 1
            select row 3 -- Assuming "Mouse & Trackpad" is the third row, adjust as needed
        end tell
        delay 1
        set value of slider 1 of window 1 to 5 -- Adjust the value as needed
    end tell
end tell

-- Close System Preferences
--tell application "System Preferences" to quit
