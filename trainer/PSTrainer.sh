#!/bin/sh
TRAINDIR=${HOME}/.config/pslauncher/train

# Set up training example sentences for PocketSphinxTrainer
if test ! -d ${TRAINDIR}; then
	mkdir -p ${TRAINDIR};
	cat > ${TRAINDIR}/arctic20.dic <<-"__EOF"
'EM	AH M
A	AH
A(2)	EY
ACROSS	AH K R AO S
AGAIN	AH G EH N
AGAIN(2)	AH G EY N
ALMOST	AO L M OW S T
ALWAYS	AO L W EY Z
ALWAYS(2)	AO L W IY Z
AND	AE N D
AND(2)	AH N D
APOLOGIZED	AH P AA L AH JH AY Z D
ASLEEP	AH S L IY P
AT	AE T
AURORA	ER AO R AH
AUTHOR	AO TH ER
BACK	B AE K
BALLS	B AO L Z
BE	B IY
BELIZE	B EH L IY Z
BEYOND	B IH AA N D
BEYOND(2)	B IH AO N D
BEYOND(3)	B IY AO N D
BLESS	B L EH S
BOREALIS	B AO R IY AE L AH S
BUSINESS	B IH Z N AH S
BUSINESS(2)	B IH Z N IH S
BUT	B AH T
CAME	K EY M
CASE	K EY S
CHAIR	CH EH R
CHANCES	CH AE N S AH Z
CHANCES(2)	CH AE N S IH Z
CHANGE	CH EY N JH
CHURCHILL	CH ER CH HH IH L
CHURCHILL(2)	CH ER CH IH L
CITIES	S IH T IY Z
CLUBS	K L AH B Z
COMING	K AH M IH NG
COMPANION	K AH M P AE N Y AH N
DANGER	D EY N JH ER
DEGREE	D IH G R IY
DELICATE	D EH L AH K AH T
DOWN	D AW N
ETC	EH T S EH T ER AH
EVENING	IY V N IH NG
EVER	EH V ER
EXCLAIMED	IH K S K L EY M D
FACED	F EY S T
FEET	F IY T
FIGHTER	F AY T ER
FOLLOWED	F AA L OW D
FOR	F AO R
FOR(2)	F ER
FOR(3)	F R ER
FOREVER	F ER EH V ER
FORGET	F AO R G EH T
FORGET(2)	F ER G EH T
FORT	F AO R T
FRIENDSHIP	F R EH N D SH IH P
FRIENDSHIP(2)	F R EH N SH IH P
FROM	F ER M
FROM(2)	F R AH M
GAD	G AE D
GAME	G EY M
GLAD	G L AE D
GO	G OW
GOD	G AA D
GREGSON	G R EH G S AH N
GREW	G R UW
HAND	HH AE N D
HANDS	HH AE N D Z
HANDS(2)	HH AE N Z
HATRED	HH EY T R AH D
HE	HH IY
HEAD	HH EH D
HIS	HH IH Z
HOPE	HH OW P
I	AY
I'LL	AY L
I'M	AY M
IF	IH F
IN	IH N
IT	IH T
IT'S	IH T S
JEALOUSY	JH EH L AH S IY
JUST	JH AH S T
JUST(2)	JH IH S T
LETTER	L EH T ER
LIFE	L AY F
LIKE	L AY K
LINE	L AY N
LOOKING	L UH K IH NG
LOOKS	L UH K S
LORD	L AO R D
LOSING	L UW Z IH NG
MEMORIES	M EH M ER IY Z
MEN	M EH N
MOMENT	M OW M AH N T
MY	M AY
NEED	N IY D
NEEDED	N IY D AH D
NEEDED(2)	N IY D IH D
NOT	N AA T
NOW	N AW
OF	AH V
ON	AA N
ON(2)	AO N
ONE	HH W AH N
ONE(2)	W AH N
ONLY	OW N L IY
PARTICULAR	P AA T IH K Y AH L ER
PARTICULAR(2)	P ER T IH K Y AH L ER
PHIL	F IH L
PHILIP	F IH L AH P
PHILIP(2)	F IH L IH P
PHYSIQUE	F AH Z IY K
PLAYING	P L EY IH NG
PROPOSED	P R AH P OW Z D
RAILROAD	R EY L R OW D
RIDGE	R IH JH
RIFLESHOT	R AY F AH L SH AA T
ROSE	R OW Z
SEE	S IY
SEEING	S IY IH NG
SHARPLY	SH AA R P L IY
SHOOK	SH UH K
SHORTER	SH AO R T ER
SHOVED	SH AH V D
SINGLE	S IH NG G AH L
STEELS	S T IY L Z
SUPERLATIVE	S UH P ER L AH T IH V
TABLE	T EY B AH L
THAN	DH AE N
THAN(2)	DH AH N
THAT	DH AE T
THAT(2)	DH AH T
THE	DH AH
THE(2)	DH IY
THEM	DH AH M
THEM(2)	DH EH M
THERE	DH EH R
THERE'S	DH EH R Z
THIS	DH IH S
TIME	T AY M
TO	T AH
TO(2)	T IH
TO(3)	T UW
TOM	T AA M
TRAIL	T R EY L
TURNED	T ER N D
TURNS	T ER N Z
TWENTIETH	T W EH N IY AH TH
TWENTIETH(2)	T W EH N IY IH TH
TWENTIETH(3)	T W EH N T IY AH TH
TWENTIETH(4)	T W EH N T IY IH TH
TWO	T UW
WANT	W AA N T
WANT(2)	W AO N T
WAS	W AA Z
WAS(2)	W AH Z
WAS(3)	W AO Z
WE	W IY
WHAT	HH W AH T
WHAT(2)	W AH T
WHITTEMORE	HH W IH T M AO R
WHITTEMORE(2)	W IH T M AO R
WILL	W AH L
WILL(2)	W IH L
YOU	Y UW
YOU'RE	Y UH R
YOU'RE(2)	Y UW R
YOUR	Y AO R
YOUR(2)	Y UH R
	__EOF
	# let's generate the file list file procedurally...
	# sorry for using gnu seq, hope you have it :)
	for i in $(seq -f%04.f 20); do
		echo arctic_$i > ${TRAINDIR}/arctic20.listoffiles;
	done
	cat > ${TRAINDIR}/arctic20.transcription <<-"__EOF"
<s> AUTHOR OF THE DANGER TRAIL PHILIP STEELS ETC </s> (arctic_0001)
<s> NOT AT THIS PARTICULAR CASE TOM APOLOGIZED WHITTEMORE </s> (arctic_0002)
<s> FOR THE TWENTIETH TIME THAT EVENING THE TWO MEN SHOOK HANDS </s> (arctic_0003)
<s> LORD BUT I'M GLAD TO SEE YOU AGAIN PHIL </s> (arctic_0004)
<s> WILL WE EVER FORGET IT </s> (arctic_0005)
<s> GOD BLESS 'EM I HOPE I'LL GO ON SEEING THEM FOREVER </s> (arctic_0006)
<s> AND YOU ALWAYS WANT TO SEE IT IN THE SUPERLATIVE DEGREE </s> (arctic_0007)
<s> GAD YOUR LETTER CAME JUST IN TIME </s> (arctic_0008)
<s> HE TURNED SHARPLY AND FACED GREGSON ACROSS THE TABLE </s> (arctic_0009)
<s> I'M PLAYING A SINGLE HAND IN WHAT LOOKS LIKE A LOSING GAME </s> (arctic_0010)
<s> IF I EVER NEEDED A FIGHTER IN MY LIFE I NEED ONE NOW </s> (arctic_0011)
<s> GREGSON SHOVED BACK HIS CHAIR AND ROSE TO HIS FEET </s> (arctic_0012)
<s> HE WAS A HEAD SHORTER THAN HIS COMPANION OF ALMOST DELICATE PHYSIQUE </s> (arctic_0013)
<s> NOW YOU'RE COMING DOWN TO BUSINESS PHIL HE EXCLAIMED </s> (arctic_0014)
<s> IT'S THE AURORA BOREALIS </s> (arctic_0015)
<s> THERE'S FORT CHURCHILL A RIFLESHOT BEYOND THE RIDGE ASLEEP </s> (arctic_0016)
<s> FROM THAT MOMENT HIS FRIENDSHIP FOR BELIZE TURNS TO HATRED AND JEALOUSY </s> (arctic_0017)
<s> THERE WAS A CHANGE NOW </s> (arctic_0018)
<s> I FOLLOWED THE LINE OF THE PROPOSED RAILROAD LOOKING FOR CHANCES </s> (arctic_0019)
<s> CLUBS AND BALLS AND CITIES GREW TO BE ONLY MEMORIES </s> (arctic_0020)
	__EOF
	cat > ${TRAINDIR}/arctic20.txt <<-"__EOF"
Author of the danger trail, Philip Steels, etc.
Not at this particular case, Tom, apologized Whittemore.
For the twentieth time that evening the two men shook hands.
Lord, but I'm glad to see you again, Phil.
Will we ever forget it.
God bless 'em, I hope I'll go on seeing them forever.
And you always want to see it in the superlative degree.
Gad, your letter came just in time.
He turned sharply, and faced Gregson across the table.
I'm playing a single hand in what looks like a losing game.
If I ever needed a fighter in my life I need one now.
Gregson shoved back his chair and rose to his feet.
He was a head shorter than his companion, of almost delicate physique.
Now you're coming down to business, Phil, he exclaimed.
It's the aurora borealis.
There's Fort Churchill, a rifle-shot beyond the ridge, asleep.
From that moment his friendship for Belize turns to hatred and jealousy.
There was a change now.
I followed the line of the proposed railroad, looking for chances.
Clubs and balls and cities grew to be only memories.
	__EOF
fi

# record samples
PocketSphinxTrainer $*

# begin training
cd ${TRAINDIR}

# progress bar
echo PROGRESS0.1

# these programs are the sphinxtrain tools
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
