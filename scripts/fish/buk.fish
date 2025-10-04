function __fish_buk_config_complete
    set -l current (commandline -ct)

    if string match -q '/*' -- "$current"
        set -l show_hidden false

        if string match -q '*/.*' -- "$current"; or string match -q '*/.' -- "$current"
            set show_hidden true
        end

        if test "$show_hidden" = "true"
            __fish_complete_path "$current" | grep -v '/\.buk$' | grep -v '/\.buk/'
        else
            __fish_complete_path "$current" | grep -v '/\.' | grep -v '/\.buk$' | grep -v '/\.buk/'
        end
    else
        set -l show_hidden false

        if string match -q '.*' -- "$current"
            set show_hidden true
        end

        if test "$show_hidden" = "true"
            pushd ~
            for file in .* *
                if test "$file" != "." -a "$file" != ".." -a "$file" != ".buk"
                    if test -e "$file"
                        if test -d "$file"
                            echo "~/$file/"
                        else
                            echo "~/$file"
                        end
                    end
                end
            end
            popd
        else
            pushd ~
            for file in *
                if test "$file" != ".buk"
                    if test -e "$file"
                        if test -d "$file"
                            echo "~/$file/"
                        else
                            echo "~/$file"
                        end
                    end
                end
            end
            popd
        end
    end
end

function __fish_buk_file_complete
    set -l current (commandline -ct)

    if string match -q '/*' -- "$current"
        __fish_complete_path "$current" | grep -v '/\.buk$' | grep -v '/\.buk/'
    else
        for file in .* *
            if test "$file" != "." -a "$file" != ".." -a "$file" != ".buk"
                if test -e "$file"
                    if test -d "$file"
                        echo "$file/"
                    else
                        echo "$file"
                    end
                end
            end
        end
    end
end

function __fish_buk_needs_command
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 1
        return 0
    end
    return 1
end

complete -f -c buk -n __fish_buk_needs_command -a init -d 'Initialize the current repository'
complete -f -c buk -n __fish_buk_needs_command -a config -d 'Configure the backup directory path'
complete -f -c buk -n __fish_buk_needs_command -a save -d 'Save a backup'
complete -f -c buk -n __fish_buk_needs_command -a restore -d 'Restore a backup'
complete -f -c buk -n __fish_buk_needs_command -a delete -d 'Delete a backup'

complete -f -c buk -n "__fish_seen_subcommand_from init"

complete -f -c buk -n "__fish_seen_subcommand_from config; and not __fish_seen_subcommand_from init save restore delete" -a "(__fish_buk_config_complete)"

complete -f -c buk -n "__fish_seen_subcommand_from save; and not __fish_seen_subcommand_from init config restore delete" -a "(__fish_buk_file_complete)"

complete -f -c buk -n "__fish_seen_subcommand_from restore; and not __fish_seen_subcommand_from init config save delete" -a "(__fish_buk_file_complete)"

complete -f -c buk -n "__fish_seen_subcommand_from delete; and not __fish_seen_subcommand_from init config save restore" -a "(__fish_buk_file_complete)"
