<?php 

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

initAutoload();

/**
 * Classe que implementa lógica de assinatura eletrônica de forma genérica para o Simec.
 * 
 * Esta clase é separada da modelo Simec_Assinaturaeletronica_Abstrata para simplificar
 * a atualização caso a tabela seja modificada. Basta gerar a modelo novamente como base
 * e renomear a classe adicionando o modificador abstrato.
 */
class Simec_Assinaturaeletronica_Assinatura extends Simec_Assinaturaeletronica_Abstrata
{
    //protected $arChavePrimaria = [];
    
    protected $cpf;
    protected $nome;    
    protected $usunome;    
    protected $data;
    protected $cargo;
    
    public $img = "../imagens/assinatura_eletronica/assinatura_eletronica.png";
    
    /**
     * Sobrescrevendo método para iniciar as variáveis $cpf e $nome como apelidos para
     * usucpf e usunome respectivamente.
     * 
     * Também faz uma join com a tabela de usuário para iniciar o nome e faz o tratamento
     * de strings para que apenas o primeiro caractere de cada palavra do nome fique 
     * em maiúsculo.
     * 
     * @param int $id     
     */
    public function carregarPorId( $id ){
        
        //mantendo comportamento da função original
        parent::carregarPorId( $id );
        
        //aidicionando inicialização dos alias
        $this->cpf = $this->usucpf;    
        $this->cargo = $this->asecargo;    
        
        $sql = 
        "SELECT 
            usunome

         FROM
            seguranca.usuario

         WHERE
            usucpf = '{$this->usucpf}'";
        
        //buscando nome do usuário
        $nome = $this->pegaUm($sql);
 
        //convertendo todas as letras para minusculas
        $nome = mb_strtolower($nome);
        //convertendo apenas a primiera letra para maiúscula
        $nome = ucwords($nome);
        
        $this->nome = $this->usunome = $nome;
    }
    
    /**
     * Método para abstrair a formatação de data. Já aceita como parâmetro uma string na sintaxe
     * de formtado de data da função date() do PHP.
     * 
     * @param string $formato String com formato da data na sintaxe da função date() default: 'd/m/Y'.
     * 
     * @return string String com a data formatada
     */
    public function data ($formato = 'd/m/Y') {
        if (!empty($this->asedata)) {
            
            $timestamp = strtotime($this->asedata);
            
            return date($formato, $timestamp);
        }
    }
    
    /**
     * Método para verificar os dados do usuário antes da assinatura
     * usando a api de senha do Simec
     * 
     * @param string $usucpf    CPF do usuário
     * @param string $senha     Senha do usuário (texto puro)
     * 
     * @return boolean      True se os dados de autenticação estiverem corretos e false caso contrário
     */
    public function autenticar ($usucpf, $senha) 
    {
        //verificando se os parâmetros não estão vazios
        if (empty($usucpf) || empty($senha)) {
            return false;    
        }
        
        $sql = 
        "SELECT
			u.usucpf,
            u.suscod,
            u.ususenha			
		 FROM
			seguranca.usuario u
		 WHERE
			u.usucpf = '{$usucpf}'";
        
        $usuario = (object) $this->recuperar( $sql );
        
        //verificando se o usuário não está desativado no Simec        
        if ($usuario->suscod == STATUS_PENDENTE || $usuario->suscod == STATUS_BLOQUEADO) {
            return false;
        }               
                
        if (md5_decrypt_senha( $usuario->ususenha, '' ) == $senha ) {

            return true; 
        } else {
            return false;
        }
        
        
    }
    
    /**
     * Método que cria a assinatura fazendo o insert na tabela
     * 
     * @param string CPF do usuário que será salvo na assinatura
     * 
     * @return boolean|integer O ID da assinatura criada, ou false em caso de falha
     */
    public function criarAssinatura ($usucpf, $cargo = null)
    {
        //verificando se o parâmetro é vazio
        if (empty ($usucpf)) {
            return false;
        }
        
        $cargo = empty($cargo) ? "NULL" : "'" . $cargo . "'"; 
        
        $sql =
        "INSERT INTO 
            {$this->stNomeTabela}
            (
                usucpf,
                asedata,
                asecargo
            )

         VALUES
            (
                '{$usucpf}',
                NOW(),
                {$cargo}   
            )

         RETURNING 
            aseid";
                
        $aseid = $this->pegaUm($sql);
        
        $this->commit();
        
        return $aseid;
    }
    
    /**
     * Método que faz a autenticação do usuário e cria a assinatura
     * se os dados informados pelo usuário estiverem corretos
     */
    public function assinar ($usucpf, $senha, $cargo = null) 
    {
       //autenticando os dados do usuário antes de assinar 
       if ( $this->autenticar($usucpf, $senha) ) {
           
           //se a autenticação foi feita cria uma nova assinatura
           return $this->criarAssinatura($usucpf, $cargo);
           
       } else {
           return false;
       }
    }
    
    /**
     * Método que imprime a tarja da assinautra a partir de um modelo
     * 
     * @param int   $idAssinatura   ID do registro de assinatura
     * @param array $parametros     Array de parâmetros adicionais para o modelo                                   
     * @param string $modelo        Nome do arquivo de modelo (default: assinatura_padrao.php)  
     */
    public function imprimirAssinatura ($idAssinatura, $parametros = array() ,$modelo = 'assinatura_padrao.php')
    {
        //verificandos e os parâmetros são válidos
        if ( empty($idAssinatura) || empty($modelo) ) {
            return false;
        }
        
        //carregando dados da assinatura
        $this->carregarPorId($idAssinatura);
        
        //verificando se alguma assinatura foi encontrada
        if (empty( $this->aseid )) {
            return false;
        }
        
        //filtrando string do nome do modelo
        $modelo = filter_var($modelo, FILTER_SANITIZE_STRING);
        
        //resolvendo o nome completo do arquivo de modelo
        $modelo = dirname(__FILE__).'/templates_assinatura/'.$modelo;
         
        //iniciando os parametros adicionais (se existirem)
        if (is_array($parametros) && !empty($parametros)) {
            //se o array existir e não for vazio extrai seu valores para ficarem disponíveis pro modelo
            extract($parametros);    
        }
        
        //verificando se o modelo de assinatura informato existe
        if (!file_exists($modelo)) {            
            return false;
        }
        
        //iniciando buffer de saída
        ob_start();
        
        //incluindo modelo de assinatura
        require_once $modelo;
        
        //armazenando o buffer de saída como string com o HTML gerado
        $tarja = ob_get_clean();
        
        //retornando string com o modelo de assinatura
        return $tarja;
    }
    
    public function gerarCodigo () 
    {                
        return strtoupper(substr(base_convert(sha1(uniqid(mt_rand())), 16, 36), 0, 10));
    }
    
    public function validarAssinatura ($codigoAssinatura)
    {
        
    }

    /**
     * Método para cancelar a assinatura.
     *
     * @param int $idAssinatura
     * @param string $usucpf
     * @param string $obs
     */
    public function cancelarAssinatura($idAssinatura, $usucpf = null, $obs = null)
    {
        if (empty($usucpf)) {
            $usucpf = $_SESSION['usucpf'];
        }
        
        // tratando texto de observação do cancelamento
        $obs = ! empty($obs) ? "'" . $obs . "'" : 'NULL';
        
        $this->carregarPorId($idAssinatura);
        
        $this->usucpfcancelamento = $usucpf;
        $this->aseobscancelamento = $obs;
        
        // removendo elemento do array de chaves primárias
        // (o código de validação é adicionado erroneamente por ter restrição "UNIQUE")
        // array_pop($this->arChavePrimaria);
        
        $sql = 
        "UPDATE
                {$this->stNomeTabela}
         SET
                asestatus = 'I',
                usucpfcancelamento = '{$this->usucpfcancelamento}',
                aseobscancelamento = {$this->aseobscancelamento},
                asedatacancelamento = NOW()
         WHERE
                aseid = {$this->aseid}";
        
        $this->executar($sql);
        $this->commit();
    }
}
