import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class SuggestFriend extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find the top n pairs of users in the database who have the most
    // common friends, but are not friends themselves.
    //
    // Your output will consist of a set of pairs (user1_id, user2_id)
    // No pair should appear in the result twice; you should always order the pairs so that user1_id < user2_id
    //
    // If there are ties, you should give priority to the pair with the smaller user1_id.
    // If there are still ties, give priority to the pair with the smaller user2_id.
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
            String query_friend = """
                    SELECT
                    	f1.user1_id, f1_user.first_name, f1_user.last_name,
                    	f2.user1_id, f2_user.first_name, f2_user.last_name,
                    	COUNT(*) AS common_friend_count,
                        GROUP_CONCAT(CONCAT(common_user.first_name, ' ', common_user.last_name) SEPARATOR ", ") AS common_friends
                    FROM friends AS f1
                    JOIN friends AS f2
                    JOIN User AS f1_user
                    JOIN User AS f2_user
                    JOIN User AS common_user
                    WHERE f1.user1_id < f2.user1_id
                    	AND NOT f1.user1_id = f2.user1_id
                    	AND f1.user2_id = f2.user2_id
                    	AND NOT (f1.user2_id = f2.user1_id OR f1.user1_id = f2.user2_id)
                        AND f1_user.user_id = f1.user1_id AND f2_user.user_id = f2.user1_id AND common_user.user_id = f1.user2_id
                    GROUP BY f1.user1_id, f2.user1_id
                    ORDER BY common_friend_count DESC, f1.user1_id ASC, f2.user1_id ASC
                    LIMIT 10;
                    """;


            ResultSet result = stmt.executeQuery(query_friend);

            out.println("Friend suggestion <br><br>");
            while(result.next())
            {
                int user1_userID = result.getInt("f1.user1_id");
                String user1_firstName = result.getString("f1_user.first_name");
                String user1_lastName = result.getString("f1_user.last_name");
                int user2_userID = result.getInt("f2.user1_id");
                String user2_firstName = result.getString("f2_user.first_name");
                String user2_lastName = result.getString("f2_user.last_name");
                int commonNum = result.getInt("common_friend_count");
                String commonFriends = result.getString("common_friends");

                out.println("{"+user1_userID+"}" + user1_firstName + " " + user1_lastName + " could know " + "{"+user2_userID+"}"+user2_firstName + " " + user2_lastName + " <br>");
                out.println("Because they know " + commonNum + " same friends: " + commonFriends +"<br><br>");
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
