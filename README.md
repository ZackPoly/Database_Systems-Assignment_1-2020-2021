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
