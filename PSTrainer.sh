cd $(dirname $0)

export PATH=$(pwd)/bin/:$PATH

cd lib/train

sphinx_fe -argfile ../model/hmm/feat.params -samprate 16000 -c arctic20.listoffiles -di . -do . -ei wav -eo mfc -mswav yes

pocketsphinx_mdef_convert -text ../model/hmm/mdef ./mdef.txt

bw -hmmdir ../model/hmm -moddeffn ./mdef.txt -ts2cbfn .semi. -feat 1s_c_d_dd -svspec 0-12/13-25/26-38 -cmn current -agc none -dictfn arctic20.dic -ctlfn arctic20.listoffiles -lsnfn arctic20.transcription -accumdir .

mllr_solve -meanfn ../model/hmm/means -varfn ../model/hmm/variances -outmllrfn mllr_matrix -accumdir .

map_adapt -meanfn ../model/hmm/means -varfn ../model/hmm/variances -mixwfn ../model/hmm/mixture_weights -tmatfn ../model/hmm/transition_matrices -accumdir . -mapmeanfn ./means -mapvarfn ./variances -mapmixwfn ./mixture_weights -maptmatfn ./transition_matrices

mk_s2sendump -pocketsphinx yes -moddeffn ./mdef.txt -mixwfn ./mixture_weights -sendumpfn ./sendump

pocketsphinx_mdef_convert -bin ./mdef.txt ../model/hmm/mdef

mv means variances mixture_weights transition_matrices sendump ../model/hmm
