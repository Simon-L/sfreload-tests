import("stdfaust.lib");

mysf(s) = 0, s : soundfile("sflabel[url:/path/to/soundfile.wav]", 2);

sf_l(s) = mysf(s) : !,!,_,!;
sf_r(s) = mysf(s) : !,!,!,_;
sf_len = mysf(0) : _,!,!,!;
sf_sr = mysf(0) : !,_,!,!;

out = it.raise_modulo(0, 1, sf_len) : _,! <: sf_l(_), sf_r(_);
process = out;