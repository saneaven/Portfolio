import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class Explore extends HttpServlet {
    public void init() throws ServletException {
        // Do required initialization
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("text/html");
        RequestDispatcher view = request.getRequestDispatcher("exploreView.html");
        view.forward(request, response);
    }
}
