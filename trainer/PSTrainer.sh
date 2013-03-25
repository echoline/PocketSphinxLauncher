#!/bin/sh

PocketSphinxTrainer $*

cd lib/train

echo PROGRESS0.1

sphinx_fe -argfile ../model/hmm/feat.params -samprate 16000 -c arctic20.listoffiles -di . -do . -ei wav -eo mfc -mswav yes

echo PROGRESS0.2

pocketsphinx_mdef_convert -text ../model/hmm/mdef ./mdef.txt

echo PROGRESS0.3

bw -hmmdir ../model/hmm -moddeffn ./mdef.txt -ts2cbfn .semi. -feat 1s_c_d_dd -svspec 0-12/13-25/26-38 -cmn current -agc none -dictfn arctic20.dic -ctlfn arctic20.listoffiles -lsnfn arctic20.transcription -accumdir .

echo PROGRESS0.4

mllr_solve -meanfn ../model/hmm/means -varfn ../model/hmm/variances -outmllrfn mllr_matrix -accumdir .

echo PROGRESS0.5

map_adapt -meanfn ../model/hmm/means -varfn ../model/hmm/variances -mixwfn ../model/hmm/mixture_weights -tmatfn ../model/hmm/transition_matrices -accumdir . -mapmeanfn ./means -mapvarfn ./variances -mapmixwfn ./mixture_weights -maptmatfn ./transition_matrices

echo PROGRESS0.6

mk_s2sendump -pocketsphinx yes -moddeffn ./mdef.txt -mixwfn ./mixture_weights -sendumpfn ./sendump

echo PROGRESS0.7

pocketsphinx_mdef_convert -bin ./mdef.txt ../model/hmm/mdef

echo PROGRESS0.8

mv means variances mixture_weights transition_matrices sendump ../model/hmm

echo PROGRESS1.0
