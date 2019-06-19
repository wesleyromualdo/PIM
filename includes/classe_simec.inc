<?php

class cls_classes_simec
{
    var $mess_erro;
    var $ultimo_erro;
    function __construct()
    {
        $this->mess_erro[101] = "Não foi possível estabelecer uma conexão com o servidor LDAP.";
        $this->mess_erro[102] = "Não foi possível autenticar no servidor LDAP.";
        $this->mess_erro[103] = "A pesquisa não retornou resultado.";
        $this->mess_erro[104] = "Não foi possível estabelecer uma conexão com o cls_banco de Dados.";
        $this->mess_erro[105] = "Falha na seleção da base de dados.";
        $this->mess_erro[106] = "Falha na execução da operação. Foi enviada uma mensagem para o Administrador.";
        $this->mess_erro[107] = "Usuário ou senha inválida!";
        $this->mess_erro[108] = "Ocorreu um erro durante a execução do Bind.";
        $this->mess_erro[200] = "Registro cancelado.";
    }

    function erro($codErro, $param)
    {
        return $this->mess_erro[$codErro];
    }
}
