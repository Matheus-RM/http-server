
function responseFailCallback(request, status, error) {

	if(status == "timeout")
		console.log("The server is not responding.");

	else if (status == "error")
		console.log("Request error:", error);
}

function loadLanguage() {

	const file = $("meta[name=language-file]").attr("content");
	const language = $("#language-selector").find(":selected").val();

	console.log("file: \"" + file + "\".");
	console.log("language: \"" + language + "\".");

	let request = $.get("/lang/" + language + "/" + file);

	request.done(function(data) {

		console.log("data:", JSON.stringify(data));

		$("html").attr("lang", language);

		Object.entries(data).forEach((entry) => {

			const [id, text] = entry;

			$("#" + id).text(text);
		});
	});

	request.fail(responseFailCallback);
}


