import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class HomeBodies extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find the users who still live in their hometowns
    // (I.e., current_city = hometown_city)
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
            String query_lonely = """
                    SELECT *
                    FROM user
                    WHERE current_city = hometown_city;
                    """;


            ResultSet result = stmt.executeQuery(query_lonely);

            out.println("Home is where the heart is! The people who are still in my hometown are... <br><br>");
            while(result.next())
            {
                int userID = result.getInt("user_id");
                String firstName = result.getString("first_name");
                String lastName = result.getString("last_name");
                String homeTown = result.getString("current_city");

                out.println("{"+userID+"} "+firstName + " " + lastName + " living at " + homeTown + "<br>");
            }
        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}