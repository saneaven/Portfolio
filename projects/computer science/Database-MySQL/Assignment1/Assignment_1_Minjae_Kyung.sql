/* Assignment 2 */
/* Minjae Kyung */

-- Run the create_rare_books.sql script to create the initialized tables and data.
-- Each question is worth 10 points.

use rarebooks;

-- Exercise 1
-- Show the count of books currently in stock for each 
-- condition description (i.e., Poor, Good, Fine, etc.).
-- Hint: For this problem you need to find books that have Volumes that do Not have sales!
-- Expected results

/* condition_description, count
	'New', '10'
	'Excellent', '15'
	'Fine', '3'  */
    
    SELECT condition_description, COUNT(volume.condition_code) AS count
    FROM volume
    INNER JOIN condition_codes
    ON volume.condition_code = condition_codes.condition_code
    WHERE sale_id is NULL
    GROUP BY volume.condition_code
    ORDER BY volume.condition_code ASC;

-- Exercise 2
-- Show the total book sales for each condition description and
-- order the result by the highest sales first.
-- Hint: Sales means how much profit I gain from selling books, 
-- NOT how many books I sold

	describe volume;
    
    SELECT * FROM volume;
    
    SELECT condition_description, SUM(volume.selling_price)
    FROM volume
    INNER JOIN condition_codes
    ON volume.condition_code = condition_codes.condition_code
    WHERE selling_price IS NOT NULL
    GROUP BY volume.condition_code
    ORDER BY SUM(volume.selling_price) DESC;

-- Exercise 3
-- Find the inventory count for every book in stock by author name 
-- and book title.  Sort the result by author name then title, both
-- in ascending order.
-- Hint: For this problem you need to find books that have Volumes that do Not have sales!
-- Some of the results (not all)
	# author, title, count
/* 	Asimov, Isaac, Foundation, 8
	Asimov, Isaac, I, Robot, 2
	Asimov, Isaac, Last Foundation, 4
	Barth, John, Lost in the Funhouse, 1
	Barth, John, Sot Weed Factor, The, 1 */
	DESCRIBE book;
    DESCRIBE volume;
    
	SELECT author.author_last_first, work.title, count(work.title) AS count
    FROM volume
    INNER JOIN book
    ON volume.ISBN = book.ISBN
    INNER JOIN work
    ON book.work_numb = work.work_numb
    INNER JOIN author
    ON work.author_numb = author.author_numb
    WHERE sale_id IS NULL
    GROUP BY author.author_last_first, work.title
    ORDER BY author.author_last_first ASC, work.title ASC;

-- Exercise 4
-- Get the count of books in stock from each publisher.  Show the
-- ISBN, title, publisher name, edition, binding, copyright year,
-- and count of books in stock, ordered by publisher name ascending.
-- Hint: For this problem you need to find books that have Volumes that do Not have sales!
	
    SELECT book.ISBN, work.title, publisher.publisher_name, book.edition, book.binding, book.copyright_year, count(book.ISBN)
    FROM volume
    JOIN book
    ON volume.ISBN = book.ISBN
    JOIN publisher
    ON book.publisher_id = publisher.publisher_id
    JOIN work
    ON book.work_numb = work.work_numb
    WHERE sale_id IS NULL
    GROUP BY book.ISBN, work.title, publisher.publisher_name, book.edition, book.binding, book.copyright_year
    ORDER BY publisher.publisher_name;
    

-- Exercise 5
-- Generate a sales report showing the daily sales for all book 
-- sales in the year 2021, with subtotals for month and year.
-- Hint: Here's last rows from the answer, there is more rows returned but hopefully this helps
-- What we want to see is on each day of the year what were the sales
-- To get each column separated you need to parse out the year, month and day our of the date using whatever functions you use.
-- The most challenging in this problem is to think about is how do you group by and rollup correctly to get the answers rolled up by year and month? 
	
    /* year	month	day	sales
	2021	7	10	455.95
	2021	7	12	100.00
	2021	7	25	230.00
	2021	7		1475.95
	2021	8	22	100.00
	2021	8		100.00
	2021	9	1	170.00
	2021	9		170.00
	2021	10	13	510.00
	2021	10		510.00
	2021			2658.95 */


SELECT YEAR(sale_date) AS year, MONTH(sale_date) AS month, DAY(sale_date) AS day, SUM(sale_total_amt) AS sales
FROM sale
WHERE YEAR(sale_date) = 2021
GROUP BY YEAR(sale_date), MONTH(sale_date), DAY(sale_date)
UNION ALL
SELECT YEAR(sale_date), MONTH(sale_date), NULL, SUM(sale_total_amt)
FROM sale
WHERE YEAR(sale_date) = 2021
GROUP BY YEAR(sale_date), MONTH(sale_date)
UNION ALL
SELECT YEAR(sale_date), NULL, NULL, SUM(sale_total_amt)
FROM sale
WHERE YEAR(sale_date) = 2021
GROUP BY YEAR(sale_date)
ORDER BY (month IS NULL), month ASC, (day IS NULL), day ASC;


-- Exercise 6
-- Show the book inventory id, title, condition description and
-- selling price for books that had a selling price higher 
-- than the average selling price of books sold in July, 2021, 
-- sorted by title in ascending order.

SELECT * FROM sale;

SELECT volume.inventory_id, work.title, condition_codes.condition_description, volume.selling_price
FROM volume
JOIN book ON volume.ISBN = book.ISBN
JOIN work ON book.work_numb = work.work_numb
JOIN condition_codes ON volume.condition_code = condition_codes.condition_code
JOIN sale ON volume.sale_id = sale.sale_id
HAVING volume.selling_price > (
	SELECT avg(sale.sale_total_amt)
	FROM sale
	WHERE YEAR(sale.sale_date) = 2021 AND MONTH(sale.sale_date) = 07
    )
ORDER BY work.title ASC;

-- Exercise 7
-- Generate the best seller list for July, 2021
-- Along with author and title, give the number of copies sold
-- and the total sales amount for each book.  Order the
-- result by copies sold in descending order.
-- Hint: You might try writing the query for copies sold first

SELECT * FROM sale;

SELECT author.author_last_first AS author, work.title AS title, count(work.title) AS number_of_copies_sold, SUM(volume.selling_price) AS total_sales
FROM volume
JOIN book ON volume.ISBN = book.ISBN
JOIN work ON book.work_numb = work.work_numb
JOIN author ON author.author_numb = work.author_numb
JOIN sale ON volume.sale_id = sale.sale_id
WHERE YEAR(sale.sale_date) = 2021 AND MONTH(sale.sale_date) = 7
GROUP BY author.author_last_first, work.title;




-- Exercise 8
-- Give statements to insert a new sale for customer id 3 of the 
-- volume with inventory_id 67 on 11/3/2021 for $125 with credit 
-- card number 1234 5678 9101 4321, expiration month 7, expiration 
-- year 23.
-- Hint: you need to also insert into sales table and update volume table. 
-- Use LAST_INSERT_ID() rather than directly get the sales_id from table.
-- Google LAST_INSERT_ID() if you don't know what it is!
-- INSERT INTO sale
-- UPDATE volume

INSERT INTO sale(customer_numb, sale_date, sale_total_amt, credit_card_numb, exp_month, exp_year) VALUES (3,"2021-11-3", 125.00, "1234 5678 9101 4321", 7, 23);
UPDATE volume
SET selling_price = 125, sale_id = LAST_INSERT_ID()
WHERE inventory_id = 67;

SELECT *
FROM sale
WHERE customer_numb = 3;

SELECT *
FROM volume
WHERE inventory_id = 67;

-- Exercise 9
-- Add the JK Rowling book "Harry Potter and Sorcerer's Stone"
-- to the rare book inventory.  This first edition book acquired
-- on 3/1/2018 is in excellent condition and has the ISBN 
-- 978-0-78622-272-8, a leather binding, the copyright year 1999 
-- from the publisher Thorndike Press and an asking price of $100.
-- Hint:
-- Following tables needs to have data inserted: author, work, book, publisher, volumne
-- Following tables needs to have data updated: book

INSERT author (author_last_first) 
VALUES ("Rowling, J.K.");
INSERT work (author_numb, title) 
VALUES (LAST_INSERT_ID(), "Harry Potter and Sorcerer's Stone");
INSERT book (ISBN, work_numb, edition, binding, copyright_year) 
VALUES ("978-0-78622-272-8", LAST_INSERT_ID(), 1, "Leather", 1999);
INSERT publisher (publisher_name) 
VALUES ("Thorndike Press");
UPDATE book SET publisher_id = LAST_INSERT_ID() WHERE ISBN = "978-0-78622-272-8";
INSERT volume (ISBN, condition_code, date_acquired, asking_price)
VALUES ("978-0-78622-272-8", 2, "2018-3-1", 100);

SELECT * FROM volume WHERE ISBN = "978-0-78622-272-8";

-- Exercise 10
-- Delete the publisher Thorndike Press from the Rare Books 
-- inventory.  Make sure to handle the case in which there are 
-- more than one book in inventory from Thorndike PRess
-- Hint: 3 tables needs to have records removed!


DELETE FROM volume
WHERE ISBN = (
	SELECT ISBN 
    FROM book 
    WHERE publisher_id = (
		SELECT publisher_id 
        FROM publisher 
        WHERE publisher_name = "Thorndike Press"
        )
	);
    
DELETE FROM book
WHERE publisher_id = (
	SELECT publisher_id 
    FROM publisher 
    WHERE publisher_name = "Thorndike Press"
    );

SELECT * FROM publisher;

DELETE FROM publisher
WHERE publisher_id = 6;

