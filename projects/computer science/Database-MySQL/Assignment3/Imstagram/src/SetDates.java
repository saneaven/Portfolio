import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class SetDates extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Given the ID of Leah (user_id is 37), find information about that
    // Leah's oldest friend and youngest friend
    //
    // If two users have exactly the same age, meaning that they were born
    // on the same day, then assume that the one with the larger user_id is older
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
            Statement stmt = conn.createStatement(
                    ResultSet.TYPE_SCROLL_INSENSITIVE,
                    ResultSet.CONCUR_READ_ONLY
            );
            // TODO: Implement your logic here!
            String query_friend = """
                    SELECT User.*
                    FROM friends
                    JOIN User
                    WHERE friends.user1_id = 37 -- Leah's ID
                    	AND friends.user2_id = User.user_id
                    ORDER BY User.birthday ASC, User.user_id DESC;
                    """;


            ResultSet result = stmt.executeQuery(query_friend);

            out.println("Info about Leah's friend! <br><br>");
            if (result.first()) {
                int userID = result.getInt("User.user_id");
                String firstName = result.getString("User.first_name");
                String lastName = result.getString("User.last_name");
                String birthday = result.getString("User.birthday");

                out.println("{"+userID+"} "+ firstName + " " + lastName + " is Leah's oldest friend born at " + birthday + "! <br><br>");
            }

            if (result.last()) {
                int userID = result.getInt("User.user_id");
                String firstName = result.getString("User.first_name");
                String lastName = result.getString("User.last_name");
                String birthday = result.getString("User.birthday");

                out.println("{"+userID+"} "+ firstName + " " + lastName + " is Leah's youngest friend born at " + birthday + "! <br><br>");
            }
        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
