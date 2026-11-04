DROP DATABASE IF EXISTS Imstagram;
CREATE DATABASE IF NOT EXISTS Imstagram;
USE Imstagram;

CREATE TABLE User_Profile
(
	user_id INTEGER,
    first_name VARCHAR(16),
    last_name VARCHAR(32),
    date_of_birth DATE,
    gender CHAR(5),
    PRIMARY KEY (user_id)
);

CREATE TABLE Education_History
(
	user_id INTEGER, 
    institution_name VARCHAR(32), 
    graduation_year INTEGER, 
    major CHAR(16), 
    degree CHAR(32),
    PRIMARY KEY(user_id),
    FOREIGN KEY(user_id) REFERENCES User_Profile(user_id)
);

CREATE TABLE Location
(
	location_id INTEGER, 
    city CHAR(16), 
    state CHAR(16), 
    country CHAR(16),
    PRIMARY KEY(location_id)
);

CREATE TABLE Hometown_Location
(
	user_id INTEGER, 
    location_id INTEGER,
    PRIMARY KEY(user_id),
    FOREIGN KEY(user_id) REFERENCES User_Profile(user_id),
    FOREIGN KEY(location_id) REFERENCES Location(location_id)
);

CREATE TABLE Current_Location
(
	user_id INTEGER, 
    location_id INTEGER,
    PRIMARY KEY(user_id),
    FOREIGN KEY(user_id) REFERENCES User_Profile(user_id)
);

CREATE TABLE Basic_Event
(
	event_id INTEGER, 
    event_creator_id INTEGER, 
    event_location_id INTEGER,
    event_name VARCHAR(32), 
    event_host_first_name VARCHAR(16), 
    event_host_last_name VARCHAR(32), 
    event_start_time TIME,
    event_end_time time,
    PRIMARY KEY (event_id),
    FOREIGN KEY (event_creator_id) REFERENCES User_Profile(user_id),
    FOREIGN KEY (event_location_id) REFERENCES Location(location_id)
);

CREATE TABLE Event_Participants
(
	user_id INTEGER, 
    event_id INTEGER,
    current_status CHAR(12),
	PRIMARY KEY (user_id, event_id),
    FOREIGN KEY (user_id) REFERENCES User_Profile(user_id),
    FOREIGN KEY (event_id) REFERENCES Basic_Event(event_id)
);

CREATE TABLE Message
(
	text_id INTEGER, 
	sender_id INTEGER, 
	reciever_id INTEGER, 
	message_content VARCHAR(256), 
	sent_time TIME,
    PRIMARY KEY (text_id),
    FOREIGN KEY (sender_id) REFERENCES User_Profile(user_id),
    FOREIGN KEY (reciever_id) REFERENCES User_Profile(user_id)
);

CREATE TABLE Album
(
	album_id INTEGER, 
	owner_id INTEGER, 
	cover_photo_id INTEGER, 
	album_name VARCHAR(32), 
	album_created_date DATE, 
	album_created_time TIME, 
	album_link VARCHAR(64), 
	album_visibility BOOL,
    PRIMARY KEY (album_id),
    FOREIGN KEY (owner_id) REFERENCES User_Profile(user_id)
);

CREATE TABLE Photo
(
	photo_id INTEGER, 
    album_id INTEGER, 
    photo_caption VARCHAR(64), 
    photo_created_time TIME,
    photo_modified_time TIME, 
    photo_link VARCHAR(64),
	PRIMARY KEY (photo_id),
    FOREIGN KEY (album_id) REFERENCES Album(album_id)
);

ALTER TABLE Album
ADD CONSTRAINT fk_cover_photo FOREIGN KEY (cover_photo_id) REFERENCES Photo(photo_id);

CREATE TABLE Photo_Tags
(
	photo_id INTEGER, 
    subject_id INTEGER, 
    tag_x_coordinate FLOAT, 
    tag_y_coordinate FLOAT, 
    tag_create_time TIME,
    PRIMARY KEY (photo_id, subject_id),
    FOREIGN KEY (photo_id) REFERENCES Photo(photo_id)
);

SHOW TABLES;