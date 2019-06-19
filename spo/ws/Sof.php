<?php
/**
 * Classe base com as configurações comuns para os webservices da SOF.
 * $Id: Sof.php 145626 2018-11-06 13:21:57Z victormachado $
 */


/**
 * @see Simec_BasicWS
 */
require(APPRAIZ . 'includes/library/simec/BasicWS.php');

/**
 * Classe base com as configurações dos webservices da SOF.
 * @abstract
 */
abstract class Spo_Ws_Sof extends Simec_BasicWS
{
    /**
     * Credenciais do usuário usadas na requisições ao WS.
     * @var credencialDTO
     */
    protected $credenciais;

    /**
     * {@inheritdoc}
     */
    protected $logRequests = true;

    /**
     * Nome do esquema que irá salvar o log de consultas.
     * @var string
     */
    protected $dbSchema;

    /**
     * Construtor especial SPO com indicação do esquema do banco de log de transações.
     * @param string $dbSchema Esquema do banco de dados com a tabela logws - se for vazio, desliga o log de requisições.
     * @param int $env Ambiente de conexão com o webservice. Vejas as variáveis de ambiente em Basic_WS.
     * @see Simec_BasicWS::PRODUCTION
     * @see Simec_BasicWS::STAGING
     * @see Simec_BasicWS::DEVELOPMENT
     */
    public function __construct($dbSchema, $env = null)
    {
        if (is_null($env)) {
            $env = (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin'])) ? self::STAGING:self::PRODUCTION;
        }

        if (empty($dbSchema)) {
            $this->setLoggerOff();
        } else {
            $this->dbSchema = $dbSchema;
        }
        parent::__construct($env);
    }

    /**
     * Inicializando a URL do ws e definindo os dados de conexão.
     */
    protected function _init()
    {
        $this->loadURL();
        $this->setCredenciais('wsmec', 'Ch0c014t3', 32);
    }

    /**
     * Define as credenciais de usuário a serem utilizadas durante as requisições.
     *
     * @param string $usuario Login do usuário.
     * @param string $senha Senha do usuário.
     * @param string $perfil Perfil do usuário.
     */
    protected function setCredenciais($usuario, $senha, $perfil)
    {
        if (class_exists('credencialDTO')) {
            $this->credenciais = new credencialDTO();
            $this->credenciais->usuario = $usuario;
            $this->credenciais->senha = md5($senha);
            $this->credenciais->perfil = $perfil;
        }
    }

    /**
     * {@inheritdoc}
     */
    protected function loadConnectionOptions()
    {
        libxml_disable_entity_loader(false);
        $options = new Simec_SoapClient_Options();
        $options->add('exceptions', true)
            ->add('trace', true)
            ->add('encoding', 'ISO-8859-1')
            ->add('cache_wsdl', WSDL_CACHE_NONE)
            ->add('soap_version', SOAP_1_2)
            ->add('ssl_method', SOAP_SSL_METHOD_SSLv23);

        switch ($this->enviroment) {
            case self::PRODUCTION;
                $options->add('local_cert', APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem")
                    ->add('passphrase', 'simec');
                break;
            case self::STAGING:
            case self::DEVELOPMENT:
                $options->add('local_cert', APPRAIZ . "planacomorc/modulos/sistema/comunica/WS_SISMEC_2.pem")
                    ->add('passphrase', 'sismec');
                break;
        }

        return $options;
    }

    /**
     * {@inheritdoc}
     */
    protected function loadClassMap()
    {
        $classMap = new Simec_SoapClient_ClassMap();
        $className = get_class($this);
        foreach (call_user_func(array("{$className}Map", 'getClassMap')) as $type => $class) {
            $classMap->add($type, $class);
        }
        return $classMap;
    }

    protected function connect()
    {
        global $db;
        parent::connect();

        if ($this->logRequests) {
            $this->getSoapClient()->startLogger(
                'db',
                array(
                    'tableName' => "{$this->dbSchema}.logws",
                    'fieldMap' => array(
                        'requestContent' => 'lwsrequestcontent',
                        'requestHeader' => 'lwsrequestheader',
                        'requestTimestamp' => 'lwsrequesttimestamp',
                        'responseContent' => 'lwsresponsecontent',
                        'responseHeader' => 'lwsresponseheader',
                        'responseTimestamp' => 'lwsresponsetimestamp',
                        'url' => 'lwsurl',
                        'method' => 'lwsmetodo',
                        'ehErro' => 'lwserro',
                    ),
                    'dbConnection' => $db
                )
            );
        }
    }

    /**
     * Este método deve armazenar no atributo urlWSDL a URL de acesso ao webservice de acordo com
     * o ambiente selecionado no construtor.
     */
    protected abstract function loadURL();
}

class CredencialDTO
{
	public $perfil; // -- int
	public $senha; // -- string
	public $usuario; // -- string
}

class BaseDTO
{
}

class RetornoDTO
{
	public $mensagensErro; // -- string
	public $sucesso; // -- boolean
}

class RetornoAcoesDTO
{
	public $registros; // -- AcaoDTO
	public $acoes; // -- Acoes
}

class AcaoDTO
{
	public $identificadorUnico; // -- int
	public $exercicio; // -- int
	public $codigoMomento; // -- int
	public $codigoProduto; // -- int
	public $codigoTipoInclusaoAcao; // -- int
	public $codigoAcao; // -- string
	public $titulo; // -- string
	public $baseLegal; // -- string
	public $finalidade; // -- string
	public $descricao; // -- string
	public $unidadeResponsavel; // -- string
	public $detalhamentoImplementacao; // -- string
	public $formaAcompanhamento; // -- string
	public $identificacaoSazonalidade; // -- string
	public $insumosUtilizados; // -- string
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoIniciativa; // -- string
	public $codigoFuncao; // -- string
	public $codigoSubFuncao; // -- string
	public $codigoOrgao; // -- string
	public $codigoEsfera; // -- string
	public $codigoTipoAcao; // -- string
	public $codigoUnidadeMedida; // -- string
	public $especificacaoProduto; // -- string
	public $beneficiario; // -- string
	public $snDireta; // -- boolean
	public $snDescentralizada; // -- boolean
	public $snLinhaCredito; // -- boolean
	public $snTransferenciaObrigatoria; // -- boolean
	public $snTransferenciaVoluntaria; // -- boolean
	public $snExclusaoLogica; // -- boolean
	public $snRegionalizarNaExecucao; // -- boolean
	public $snAquisicaoInsumoEstrategico; // -- boolean
	public $snParticipacaoSocial; // -- boolean
	public $snAcompanhamentoOpcional; // -- boolean
	public $localizadores; // -- Localizadores
}

class Localizadores
{
	public $localizador; // -- LocalizadorDTO
}

class LocalizadorDTO
{
	public $codigoLocalizador; // -- string
	public $codigoMomento; // -- int
	public $codigoRegiao; // -- int
	public $codigoTipoInclusao; // -- int
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $exercicio; // -- int
	public $identificadorUnico; // -- int
	public $identificadorUnicoAcao; // -- int
	public $justificativaRepercussao; // -- string
	public $mesAnoInicio; // -- dateTime
	public $mesAnoTermino; // -- dateTime
	public $municipio; // -- string
	public $snAcompanhamentoOpcional; // -- boolean
	public $snExclusaoLogica; // -- boolean
	public $totalFinanceiro; // -- double
	public $totalFisico; // -- double
	public $uf; // -- string
}

class Acoes
{
	public $acao; // -- string
}