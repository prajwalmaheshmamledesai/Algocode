package com.mongodb;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.bson.Document;
import org.bson.conversions.Bson;
import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.annotate.JsonMethod;
import org.codehaus.jackson.annotate.JsonAutoDetect.Visibility;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.ser.ArraySerializers;

import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Filters.*;

/**
 * Hello world!
 *
 */

public class App {

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		json.setVisibility(JsonMethod.FIELD, Visibility.ANY);
		MongoClientOptions options = new MongoClientOptions.Builder().connectionsPerHost(10).build();
		MongoClient client = new MongoClient(new ServerAddress("localhost", 27017), options);

		MongoDatabase database = client.getDatabase("video");

		MongoCollection<Document> collection = database.getCollection("movie");
		List<Document> documents = collection.find().into(new ArrayList<Document>());

		for (Document document : documents) {
			System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(document));
		}

		Document document = new Document("name", "abc").append("year", "1947");

		Document document1 = new Document("name", "abc1").append("year", "1947");

		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(document));

		List<Document> list1 = new ArrayList<Document>();
		list1.add(document1);
		list1.add(document);

		System.out.println(collection.find().first());

		// Bson fiter = new Document("name", "abc");

		Bson fiter = Filters.and(Filters.eq("year", "2016"), Filters.eq("name", "Prajwal"));

		MongoCursor<Document> cursor = collection.find(fiter).iterator();

		try {
			while (cursor.hasNext()) {
				Document doc = (Document) cursor.next();
				System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(doc));
			}
		} finally {
			cursor.close();
		}
		System.out.println(collection.count(fiter));
	}
}
