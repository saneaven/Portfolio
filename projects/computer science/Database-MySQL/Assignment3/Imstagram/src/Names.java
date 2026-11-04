import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class Names extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find information about users' names:
    // (1) The longest first name (if there is a tie, include all in result)
    // (2) The shortest first name (if there is a tie, include all in result)
    // (3) The most common first name, and the number of times it appears (if there is a tie, include all in result
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
            String query_max = """
                    SELECT * FROM User
                    Where char_length(first_name) = (
                    	SELECT MAX(CHAR_LENGTH(first_name))
                        AS max_length
                    	FROM User
                    );""";

            String query_min = """
                    SELECT * FROM User
                    Where char_length(first_name) = (
                    	SELECT MIN(CHAR_LENGTH(first_name))
                        AS max_length
                    	FROM User
                    );""";

            String query_freq_num = """
                    SELECT MAX(name_count) AS max_num
                    FROM (
                    	SELECT first_name, COUNT(*) as name_count
                    	FROM User
                    	GROUP BY first_name
                    ) AS Name_counts""";
            ResultSet result = stmt.executeQuery(query_freq_num);

            int freq_num = -1;

            while(result.next())
            {
                freq_num = result.getInt("max_num");
            }

            String query_freq = """
                    SELECT * FROM User
                    WHERE first_name IN (
                    	SELECT first_name
                    	FROM User
                    	GROUP BY first_name
                    	HAVING COUNT(*) = """ + freq_num + """
                    );""";


            result = stmt.executeQuery(query_max);

            out.println("User(s) that has most longest first name is...<br><br>");
            while(result.next())
            {
                int userID = result.getInt("user_id");
                String firstName = result.getString("first_name");
                String lastName = result.getString("last_name");

                out.println("{"+userID+"} "+firstName + " " + lastName + "<br>");
            }

            out.println("<br><br>User(s) that has most shortest first name is...<br><br>");
            result = stmt.executeQuery(query_min);

            while(result.next())
            {
                int userID = result.getInt("user_id");
                String firstName = result.getString("first_name");
                String lastName = result.getString("last_name");

                out.println("{"+userID+"} "+firstName + " " + lastName + "<br>");
            }

            out.println("<br><br>The most common first name was used "+ freq_num +" times, and those people are...<br><br>");
            result = stmt.executeQuery(query_freq);

            while(result.next())
            {
                int userID = result.getInt("user_id");
                String firstName = result.getString("first_name");
                String lastName = result.getString("last_name");

                out.println("{"+userID+"} "+firstName + " " + lastName + "<br>");
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
