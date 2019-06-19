<?php
defined('APPRAIZ') || define('APPRAIZ', '/var/www/html/simec/');

if (!empty($_POST)) {

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, 'https://siop.planejamento.gov.br/modulo/auth/login');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    // curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
    // curl_setopt($ch, CURLOPT_TIMEOUT, 5);
    // Para que não haja validação de entidade certificadora para o certificado
    // as opções CURLOPT_SSL_VERIFYHOST e CURLOPT_SSL_VERIFYPEER precisam estar 'false'.
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_VERBOSE, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, "username={$_POST['cpf']}&password={$_POST['pwd']}");
    $data = curl_exec($ch);
    $httpcode = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    echo "Resultado:<br/><br/>";
    echo $data;

    // $ch = curl_init('https://siop.planejamento.gov.br/modulo/auth/login');
    // curl_setopt_array(
    //     $ch,
    //     [
    //         CURLOPT_POST => true,
    //         CURLOPT_RETURNTRANSFER => true,
    //         //CURLOPT_SSL_VERIFYPEER => false,
    //         CURLOPT_SSL_VERIFYHOST => 1,
    //         CURLOPT_SSL_VERIFYPEER => 1,
    //         CURLOPT_HEADER => true,
    //         CURLINFO_HEADER_OUT => true,
    //         CURLOPT_POSTFIELDS => "username={$_POST['cpf']}&password={$_POST['pwd']}",
    //         CURLOPT_VERBOSE => true,
    //         CURLOPT_HTTP_VERSION => 4,
    //         CURLOPT_SSLCERT => APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem",
    //     ]
    // );

    // echo "<h3>Execução</h3>";
    // var_dump(curl_exec($ch));

    echo "<h3>Erros</h3>";
    var_dump(curl_error($ch));

    echo "<h3>Erros código</h3>";
    var_dump(curl_errno($ch));

    echo "<h3>Info</h3>";
    var_dump(curl_getinfo($ch));
    curl_close($ch);
    return;
}
?>
<!DOCTYPE html>
<html>
<body>

<form method="POST">
  CPF <b>(000.000.000-00)</b>:<br>
  <input type="text" name="cpf">
  <br>
  Senha:<br>
  <input type="password" name="pwd">
  <br><br>
  <input type="submit" value="Enviar">
</form>
</body>
</html>
