import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class PhotoTag extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find the top-8 photos based on the number of tagged users
    // If there are ties, choose the photo with the smaller numeric PhotoID first
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
            String query_topTag = """
                    SELECT
                    	tagged_photo_user.photo_id,
                        GROUP_CONCAT(CONCAT(User.first_name, ' ', User.last_name) ORDER BY User.first_name SEPARATOR ', ') AS tagged_users,
                        photo.city,
                        COUNT(tagged_photo_user.photo_id) AS tagged_num
                    FROM tagged_photo_user
                    LEFT JOIN User
                    On tagged_photo_user.tagged_photo_user_id = User.user_id
                    LEFT JOIN photo
                    On tagged_photo_user.photo_id = photo.photo_id
                    GROUP BY tagged_photo_user.photo_id
                    ORDER BY tagged_num DESC, tagged_photo_user.photo_id ASC
                    LIMIT 8;
                    """;


            ResultSet result = stmt.executeQuery(query_topTag);

            out.println("What are the photos that people are most tagged in? Let's find out now! <br>");
            out.println("These are the top 8 list! <br><br>");
            while(result.next())
            {
                String photoID = result.getString("photo_id");
                int taggedNum = result.getInt("tagged_num");
                String taggedUsers = result.getString("tagged_users");
                String city = result.getString("city");

                out.println("Image {"+ photoID + "} at " + city +" : There are "+taggedNum+" people tagged in this photo!<br>");
                out.println("\tTagged People: " + taggedUsers + "<br><br>");
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
