### Test soundfile reload repo

Put a correct path in test.dsp, then `make main` and `make test`.  
While running, just paste a new path to the cmd ui in terminal, press Enter, and the file will be reloaded on the fly.

Warning: there is no realtime safeguards of any kind in place, this is just a basic demo. CMDUI was simply hacked to had an external handler for commands typed in the terminal by the user.

Have a look at `ReloadSoundUI.h` for some implementation info and the main function in `jack-console.cpp` fpr usage.