<?php
if (!$_POST['ajaxCPF']):
    ?>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script src="/includes/webservice/cpf.js"></script>
<?php
endif;
include_once 'config.inc';

/**
 * Classe para acesso ao webservice de pessoa fÃ­sica.
 *
 * PS: NÃ£o esqueÃ§a de ler o leiame.txt
 *
 */
final class PessoaFisicaClient
{
    /**
     * Coloca o objeto do cliente do webservice.
     *
     * @var SoapClient
     */
    private $soapClient;


    /**
     * Construtor da classe.
     *
     * @param string $wsdl
     */
    public function __construct($wsdl)
    {

        try{
            $this->soapClient = new Simec_SoapClient( $wsdl );
        } catch (Exception $e){
            exit("NÃ£o estÃ¡ conectado!");
        }

    }

    /**
     * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
     *
     * @param integer $cpf
     * @return xml
     */
    public function solicitarDadosPessoaFisicaPorCpf( $cpf )
    {
        return (  $this->soapClient->solicitarDadosPessoaFisicaPorCpf($cpf) );
    }

    /**
     * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
     *
     * @param integer $cpf
     * @return xml
     */
    public function solicitarDadosResumidoPessoaFisicaPorCpf( $cpf )
    {
        return (  $this->soapClient->solicitarDadosResumidoPessoaFisicaPorCpf( $cpf ) );
    }

    /**
     * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
     *
     * @param integer $cpf
     * @return xml
     */
    public static function solicitarDadosEnderecoPessoaFisicaPorCpf( $cpf )
    {
        return (  $this->soapClient->solicitarDadosEnderecoPessoaFisicaPorCpf( $cpf ) );
    }

    /**
     * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
     *
     * @param integer $cpf
     * @return xml
     */
    public static function solicitarDadosContatoPessoaFisicaPorCpf( $cpf )
    {
        return (  $this->soapClient->solicitarDadosContatoPessoaFisicaPorCpf( $cpf ) );
    }
}

/**
 *
 */
if ($_POST['ajaxCPF']){

    $cpf = $_POST['ajaxCPF'];
    $cpf = str_replace(array('/', '.', '-'), '', $cpf);
    /**
     * Aqui é feita a chamada do método da classe cliente do webservice.
     */
    $objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
    $xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($cpf);

    $obj = (array) simplexml_load_string($xml);

    if (!$obj['PESSOA']) {
        die();
    }

    $pessoa   = (array) $obj['PESSOA'];
    $endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
    $contato  = (array) $obj['PESSOA']->CONTATOS->CONTATO;


    foreach($pessoa as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[] = str_replace(array('"', "'"), '', "$k#{$val}");
    endforeach;

    foreach($endereco as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[] = str_replace(array('"', "'"), '', "$k#{$val}");
    endforeach;

    foreach($contato as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[] = str_replace(array('"', "'"), '', "$k#{$val}");
    endforeach;

    if($return) die(implode('|', $return));

}