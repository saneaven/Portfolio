import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class Siblings extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find pairs of potential siblings.
    //
    // A pair of users are potential siblings if they have the same last name and hometown, if they are friends, and
    // if they are less than 10 years apart in age.  Pairs of siblings are returned with the lower user_id user first
    // on the line.  They are ordered based on the first user_id and in the event of a tie, the second user_id.
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
            String query_popularCity = """
                    SELECT u1.*, u2.*
                    FROM friends
                    JOIN User AS u1
                    JOIN User AS u2
                    WHERE friends.user1_id < friends.user2_id
                    	AND u1.user_id = friends.user1_id AND u2.user_id = friends.user2_id
                        AND ABS(YEAR(u1.birthday) - YEAR(u2.birthday)) < 10
                        AND u1.last_name = u2.last_name
                    ORDER BY u1.user_id ASC, u2.user_id ASC;
                    """;


            ResultSet result = stmt.executeQuery(query_popularCity);

            out.println("Maybe they are siblings... <br><br>");
            while(result.next())
            {
                int user1_userID = result.getInt("u1.user_id");
                String user1_firstName = result.getString("u1.first_name");
                String user1_lastName = result.getString("u1.last_name");
                int user2_userID = result.getInt("u2.user_id");
                String user2_firstName = result.getString("u2.first_name");
                String user2_lastName = result.getString("u2.last_name");

                out.println("{"+user1_userID+"} "+ user1_firstName + " " + user1_lastName + " and " + "{"+user2_userID+"} "+ user2_firstName + " " + user2_lastName + "<br><br>");
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
