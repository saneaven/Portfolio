USE Imstagram;

DROP TABLE Photo_Tags;
ALTER TABLE Album
DROP FOREIGN KEY fk_cover_photo;
DROP TABLE Photo;
DROP TABLE Album;
DROP TABLE Message;
DROP TABLE Event_Participants;
DROP TABLE Basic_Event;
DROP TABLE Current_Location;
DROP TABLE Hometown_Location;
DROP TABLE Location;
DROP TABLE Education_History;
DROP TABLE User_Profile;

SHOW TABLES;