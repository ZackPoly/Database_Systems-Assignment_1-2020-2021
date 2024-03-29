# Database_Systems-Assignment_1-2020-2021
JSON Database System Implementation using C .

**Ο φάκελος Datasets πρέπει να είναι μαζί μαι τη main**

compile :
  make

run :
  ./main Datasets 2013_camera_specs sigmod_large_labelled_dataset.csv
  
Τα αποτελέσματα βρίσκονται στο Output.csv στο main branch

Δομές :
- site_hash
  hash table με τα sites του Dataset
  Το collision επιλύνεται με λίστα. Έχουμε βάλει Default buckets num = 97
  
- id_hash
  Κάθε site bucket node έχει ένα hash table με το full_id ενός προϊόντος.
  Το κάθε full_id έχει ένα δείκτη σε μία κλίκα.
  
- complex
  Η δομή της κλίκας. Έχει ένα head και ένα tail σε μία λίστα με char* full_id
  Για κάθε ζευγάρι αλλάζουμε τους pointers head,tail του πρώτου id
  και διαγράφουμε τη κλίκα του δεύτερου.
  
- specs_list
  λίστα της οποίας τα node έχουν ένα attribute και μία λίστα από values

ενέργειες main :
  - Αρχικοποίηση δομών :
    Για κάθε φάκελο στο 2013_camera_specs κάνουμε insert στο site_hash.
    Για κάθε αρχείο στο current site κάνουμε insert στο current bucket node.
    Στη δεύτερη για κάθε full_id δημιουργούμε μία κλίκα με το ίδιο και 
    διαβάζουμε το .json για τα specs του
  
  - Αλλαγή κλικών :
    Για κάθε ζευγάρι γίνεται έλεγχος αν είναι ήδη στην ίδια κλίκα.
    κάνουμε free τον compPointer2, αλλάζουμε κατάλληλα τα head/tail
    και κάνουμε compPointer2 = compPointer1
    
  - print κλίκες :
    scanάρουμε τα hash tables. Για κάθε full_id βρίσκουμε τη θέση του στη κλίκα
    και εκτυπώνουμε στο Output.csv το ίδιο με κάθε full_id που ακολουθεί.



ΜΕΡΟΣ ΔΕΥΤΕΡΟ


compile:
	make

run:
	./main Datasets 2013_camera_specs sigmod_medium_labelled_dataset.csv


Δομές:

-αρνητικές συσχετίσεις:
 λίστα, στο head κάθε κλίκας,της οποίας τα node έχουν δείκτη σε κλίκα.

-bag of words:
 δομή που περιέχει ένα hash table για τις λέξεις (dictionary) και έναν 
 διδιάστατο πίνακα ο οποίος ξεκινάει από το bag of words του κάθε αρχείου
 και το μετατρέπουμε σε πίνακα με τις τιμές idf για κάθε αρχείο.Για βελτιώσεις
 στο bag of words εκτός απο stopwords επιλέγουμε να μην κρατάμε και λέξεις που 
 εμφανίζονται μόνο μια φορά στο σύνολο των αρχείων καθώς και λέξεις με περισσότερους
 από 9 χαρακτήρες.

-model:
 δομη που περιέχει έναν πίνακα για τα βάρη και τις σταθερές(b και learning rate).
 Κάνουμε training ανά ένα ζευγάρι από το αρχείο training .


Ενέργεις main:

-Αρχικοποίηση δομών (βλ. ΜΈΡΟΣ ΠΡΏΤΟ)

-Αλλαγή κλικών (βλ. ΜΈΡΟΣ ΠΡΏΤΟ)

-Δημιουργία αρνητικών συσχετίσεων:
 αν βρούμε ζευγάρι με label 0 απλά κάνουμε append έναν κόμβο στην λίστα αρνητικών
 συσχετίσεων και των 2 κλικών που αντιστοιχούν στα προϊόντα του ζευγαριού με δείκτη 
 στην άλλη κλίκα.
 αν βρούμε ζευγάρι με label 1 (μεταβατικότητα αρνητικής συσχέτισης) για κάθε κλίκα στις
 αρνητικές συσχετίσεις της δεύτερης κλίκας του ζευγαριού,διαγράφουμε την αρνητική συσχέτιση
 με την δεύτερη κλίκα και την κάνουμε append στην πρώτη κλίκα.

-Δημιουργία αρχείων (train,validation,test):
 σκανάρουμε τα hash tables και για κάθε full_id, βρίσκουμε τη θέση του στη κλίκα και εκτυπώνουμε στο 
 κατάλληλο αρχείο (ανάλογα με την πιθανότητα) το ίδιο με κάθε full_id που ακολουθεί.Το ίδιο κάνουμε
 και για τα αρνητικά ζευγάρια.

-Εκπαίδευση μοντέλου:
 για κάθε ζευγάρι που διαβάζουμε από το αρχείο training δημιουργούμε ένα vector (απόλυτη διαφορά των
 δύο vectors) και το κάνουμε train ανά ένα ζευγάρι.

-Έλεγχος μοντέλου:
 για κάθε ζευγάρι που διαβάζουμε από το αρχείο test,υπολογίζουμε την πιθανότητα να είναι ίδια προϊόντα
 και αναλόγως αυξάνουμε τον μετρητή που κρατάει τις επιτυχημένες προσπάθειες.Εκτυπώνουμε το ποσοστό επιτυχίας.



ΜΕΡΟΣ ΤΡΙΤΟ

compile:
	make


run:
	./main Datasets 2013_camera_specs sigmod_medium_labelled_dataset.csv


Δομές:

-Job Scheduler: δομή αυτή έχει μια queue (FIFO με head και tail) για τα Jobs, υπάρχει ένα mutex για το queue για να παίρνει κάθε
thread ένα job.Το condition variable που περιμένει να τελειώσουν όλα τα jobs είναι το cvPJ και χρησιμοποιείται στην συνάρτηση 
wait_all_tasks_finish(), τα threads περιμένουν σ' αυτό το condition variable κάθε φορά που αδειάζει το queue με τα Jobs.

-Job: η δομή αυτή έχει έναν δείκτη σε συνάρτηση (train() , test()) και έναν πίνακα από ορίσματα γι αυτήν.

-arg: είναι τα ορίσματα που δίνουμε στο Master thread.


Multithreading:

Χρησιμοποιούμε 5 threads, τα οποία δημιουργούμε μετά την δημιουργία του tf-idf πίνακα (και όχι στην αρχή του προγράμματος
γιατί η παρουσία των threads από την αρχή, εμπόδιζαν την λειτουργία του προγράμματος). Στη συνέχεια δημιουργούμε και ένα
Master thread το οποίο είναι υπεύθυνο για την δημιουργία των jobs, την ανανέωση των βαρών στο τέλος κάθε epoch και υπολογίζει
το τελικό ποσοστό επιτυχίας του μοντέλου.


Σε κάθε epoch (3 στο σύνολο) :
κάθε thread παίρνει ένα train_job από την ουρά του Job Scheduler (μέχρι να αδειάσει), διαβάζει ταυτόχρονα με τα άλλα threads
το Train.csv (τόσα ζευγάρια όσο το batch size, όχι συνεχόμενα ζευγάρια) μεχρι το τέλος του αρχείου. Η getline() είναι 
thread-safe επομένως δεν χρειάζεται συγχρονισμός μεταξύ των threads για το διάβασμα. Κάνει το train και προσθέτει στην κατάλλη-
λη θέση του πίνακα derivatives, το άθροισμα όλων των παραγώγων των ζευγαριών που εξέτασε. Όταν τελειώσουν όλα τα batches το Master
thread υπολογίζει τον μέσο όρο των τιμών του πίνακα derivatives και κάνει update τα βάρη του μοντέλου. Στο τέλος ξανακάνει τις τιμές
του πίνακα μηδενικές για το επόμενο epoch.

Διαδικασία test:
κάθε thread παίρνει ένα test_job από την ουρά του Job Scheduler (μέχρι να αδειάσει), διαβάζει ταυτόχρονα με τα άλλα threads
το Test.csv (τόσα ζευγάρια όσο το batch size, όχι συνεχόμενα ζευγάρια) μεχρι το τέλος του αρχείου. Kάνει τις προβλέψεις και 
υπολογίζει το ποσοστό επιτυχίας του batch, το οποίο προσθέτει στην κατάλληλη θέση του πίνακα accuracy. Το Master thread υπολο-
γίζει το μέσο ποσοστό επιτυχίας του μοντέλου.


Μετά και την ολοκλήρωση του test δίνουμε σε κάθε thread την job_terminate, η οποία κάνει sleep το thread μέχρι αυτό να διαγραφεί.
 
Έχουμε σχολιάσει την δημιουργία των κλικών κάθως έχουμε ήδη τα αρχεία Train.csv,Test.csv και Validation.csv από το αποτέλεσμα 
της 2ης εργασίας.



Παρατηρήσεις:

-Το μεγαλύτερο χρόνο κάνει η δημιουργία του bow και η μετατροπή του σε tf-idf, ενώ το training του μοντέλου, με την χρήση των threads
γίνεται σε μηδαμινό χρόνο.

-Έχει μεγάλη επιβάρυνση στην μνήμη ο πίνακας tf-idf.

-Το μοντέλο έχει υψηλό ποσοστό επιτυχίας επειδή το dataset είναι biased ως προς τις συσχετίσεις 0.

-Οι τιμές στον πίνακα tf-idf δεν χρειάζονται normalization επειδή είναι μίκρες.

-Επειδή δεν βρήκαμε τρόπο να διορθώσουμε τα conflicts στο validation set δεν υλοποιήσαμε την σύγκλιση του μοντέλου.

-Τα linux αρκετές φορές κολλούσαν κατά την εκτέλεση του προγράμματος.



Πειράματα:

Με 1 epoch το μοντέλο είχε:
 ποσοστό επιτυχίας ~89%, ολοκληρώθηκε μετά από 11m26s και δέσμευσε 5.031.830.876 bytes.
 ποσοστό επιτυχίας ~89%, ολοκληρώθηκε μετά από 11m18s και δέσμευσε 5.031.830.876 bytes.

Με 3 epoch το μοντέλο είχε:
 ποσοστό επιτυχίας ~91%, ολοκληρώθηκε μετά από 17m8s και δέσμευσε 8.860.446.788 bytes .
 ποσοστό επιτυχίας ~92%, ολοκληρώθηκε μετά από 20m27s και δέσμευσε 8.860.446.482 bytes .


Προδιαγραφές συστήματος που τρέχαμε την εργασία:

Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              12
On-line CPU(s) list: 0-11
Thread(s) per core:  2
Core(s) per socket:  6
Socket(s):           1
NUMA node(s):        1
Vendor ID:           AuthenticAMD
CPU family:          23
Model:               1
Model name:          AMD Ryzen 5 1600 Six-Core Processor
Stepping:            1
CPU MHz:             1374.400
CPU max MHz:         3200,0000
CPU min MHz:         1550,0000
BogoMIPS:            6387.47
Virtualization:      AMD-V
L1d cache:           32K
L1i cache:           64K
L2 cache:            512K
L3 cache:            8192K
NUMA node0 CPU(s):   0-11



	     total        used        free      shared  buff/cache   available
Mem:        8166616     2249824     4645888      642512     1270904     5001640
Swap:       8323068     1487872     6835196












