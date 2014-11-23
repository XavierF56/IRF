#IRF - group members: Rebecca Jahn, Arnaud Saint-Pierre, Xavier Fraboulet, Raphael Baron

Implementation of pre-processing and image processing of handdrawn icons used by emergency services. 
First we rotated the images using the position of crosses as reference points.Then we extracted, for each line, the template image and matched it to the correct template in our database. The rest of the images of that line were extracted and saved along with a .txt file describing the position in the original picture and the name of the associated template.

Performance results: Tests on the w000-scans folder showed a success rate of 100% (if there is no possible match, no images are saved in the database)