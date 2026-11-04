import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class MatchPairs extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find suggested "match pairs" of users, using the following criteria:
    // (1) One of the users is female, and the other is male
    // (2) Their age difference is within "yearDiff"
    // (3) They are not friends with one another
    // (4) They should be tagged together in at least one photo
    //
    // You should return up to n "match pairs"
    // If there are more than n match pairs, you should break ties as follows:
    // (i) First choose the pairs with the largest number of shared photos
    // (ii) If there are still ties, choose the pair with the smaller user_id for the female
    // (iii) If there are still ties, choose the pair with the smaller user_id for the male
    // 
    // Hint: for yearDiff, you only need to compute different between years, don't consider month and day!
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        String title = "Database Result";

        String docType =
                "<!doctype html public \"-//w3c//dtd html 4.0 " + "transitional//en\">\n";

        out.println(docType +
                "<html>\n" +
                "<head><title>" + title + "</title></head>\n" +
                "<body bgcolor = \"#f0f0f0\">\n" +
                "<h1 align = \"center\">" + title + "</h1>\n");

        try {
            // Register JDBC driver
            Class.forName("com.mysql.jdbc.Driver");

            Connection conn = DriverManager.getConnection(DB_URL, USER, PASS);
            Statement stmt = conn.createStatement();
            // TODO: Implement your logic here!
            String query_pairs = """
                    SELECT male.user_id, male.first_name, male.last_name, male.birthday, female.user_id, female.first_name, female.last_name, female.birthday,
                       	count(*) AS tagged_num, GROUP_CONCAT(photo.city SEPARATOR ", ") AS city
                       FROM User AS male
                       JOIN User AS female
                       JOIN tagged_photo_user AS tagged1
                       JOIN tagged_photo_user AS tagged2
                       JOIN photo
                       WHERE male.gender = "male" AND female.gender = "female"
                       	AND ABS(YEAR(male.birthday) - YEAR(female.birthday)) < 5
                           AND NOT EXISTS (
                       		SELECT 1
                       		FROM friends
                               WHERE (male.user_id = friends.user1_id AND female.user_id = friends.user2_id)
                       			OR (female.user_id = friends.user1_id AND male.user_id = friends.user2_id)
                       		)
                       	AND (male.user_id = tagged1.tagged_photo_user_id
                       			AND female.user_id = tagged2.tagged_photo_user_id
                                   AND tagged1.photo_id = tagged2.photo_id)
                       	AND photo.photo_id = tagged1.photo_id
                       	GROUP BY male.user_id, female.user_id
                        ORDER BY tagged_num DESC, female.user_id ASC, male.user_id ASC
                           LIMIT 10;
                    """;


            ResultSet result = stmt.executeQuery(query_pairs);

            out.println("Find friends who took pictures together! <br><br>");
            while(result.next())
            {
                int male_userID = result.getInt("male.user_id");
                String male_firstName = result.getString("male.first_name");
                String male_lastName = result.getString("male.last_name");
                int female_userID = result.getInt("female.user_id");
                String female_firstName = result.getString("female.first_name");
                String female_lastName = result.getString("female.last_name");
                String location = result.getString("city");
                int meetNum = result.getInt("tagged_num");

                out.println("{"+male_userID+"}"+male_firstName + " " + male_lastName + " and " +"{"+female_userID+"}"+ female_firstName + " " + female_lastName + " <br>");
                out.println("\t They meet " + meetNum + " times at " + location +" <br><br>");
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
