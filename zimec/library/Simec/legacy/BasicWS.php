<?php
/**
 * Classe básica para acesso a webservices com tecnologia Soap.
 * @version $Id: BasicWS.php 87061 2014-09-19 13:25:44Z maykelbraz $
 */

/**
 * Cliente Soap Simecs.
 * @see Simec_SoapClient
 */
require_once(dirname(__FILE__) . '/SoapClient.php');

/**
 * Classe base de chamada a webservices.
 * @todo: Adicionar metodo para recuperar o timestamp da ultima requisicao
 *
 * Compilação de métodos e tipos retirada da classe EasyWsdl2PHP, com modificações<br />
 * para atender a esta estrutura.
 */
abstract class Simec_BasicWS
{
    const PRODUCTION = 1;
    const STAGING = 2;
    const DEVELOPMENT = 3;

    /**
     * Indica o ambiente em que o WS será consultado.
     * @var int
     */
    protected $enviroment = self::PRODUCTION;

    /**
     * URL de requisição do WS, carregado com loadURL.
     * @var string
     */
    protected $urlWSDL;

    /**
     * Referência para o client do webservice.
     * @var SoapClient
     */
    protected $soapClient;

    /**
     * Usuário utilizado no header da requisição.
     * @var string
     */
    protected $headerUsername;

    /**
     * Senha utilizada no header da requisição.
     * @var string
     */
    protected $headerPassword;

    /**
     * Indica se deve se salvo o log das requisições.
     * @var boolean
     */
    protected $logRequests = false;

    /**
     * Lista de tipos simples utilizados para fazer um reverso de dados do webservice.
     * @var array
     */
    protected $simpleTypes = array('string', 'int', 'double', 'dateTime', 'float', 'boolean', 'decimal');

    /**
     *
     * @param type $env
     */
    public function __construct($env = null)
    {
        // -- Configurando o ambiente de execução
        if (!is_null($env)) {
            $this->enviroment = $env;
        }
        $this->_init();
        $this->connect();
    }

    protected function connect()
    {
        // -- Instanciando o soap client
        $this->soapClient = new Simec_SoapClient(
            $this->urlWSDL,
            $this->loadConnectionOptions(),
            $this->loadClassMap()
        );
    }

    public function setHeaderUsername($headerUsername)
    {
        $this->headerUsername = $headerUsername;
        return $this;
    }

    /**
     * Retorna o usuário utilizado no header da requisição.
     * @return string
     */
    protected function getHeaderUsername()
    {
        return $this->headerUsername;
    }

    /**
     * Retorna a senha utilizada no header da requisição.
     * @return string
     */
    protected function getHeaderPassword()
    {
        return $this->headerPassword;
    }

    public function setHeaderPassword($headerPassword)
    {
        $this->headerPassword = $headerPassword;
        return $this;
    }

    /**
     * Usa mt_rand() para gerar um número único não repetido. Em sequida, empacota esse número como um
     * número hexadecimal e retorna este valor codificado em base64.
     * @return string
     */
    protected function getNonce()
    {
        return base64_encode(
            pack('H*', mt_rand())
        );
    }

    /**
     * Desliga o log de requisições (ligado, por padrão).
     */
    public function setLoggerOff()
    {
        $this->logRequests = false;
    }

    public function generateClassMap($force = false)
    {
        $ref = new ReflectionClass($this);
        $file = $ref->getFileName();
        $className = get_class($this);
        $file = str_replace($className, "{$className}Map", $file);

        if (!is_file($file) || $force) {
            file_put_contents($file, $this->getWSDLTypes());
            return $this;
        }
        throw new Exception("O arquivo de mapeamento '{$className}Map' já existe. Use \$force = true para sobreescrever o arquivo.");
    }

    /**
     * Cria uma lista de todas as classes de mapeamento utilizadas no webservice.
     * Método adaptado de EasyWsdl2PHP::generate()
     */
    protected function getWSDLTypes()
    {
        $types = $this->soapClient->__getTypes();
        $class = get_class($this);
        $nl = "\n";

        $codeType = '';
        foreach ($types as $type) {
            if (substr($type, 0, 6) == 'struct') {
                $data = trim(str_replace(array('{', '}'), '', substr($type, strpos($type, '{') + 1)));
                $data_members = split(';', $data);
                $classname = trim(substr($type, 6, strpos($type, '{') - 6));

                //write object
                $codeType .= $nl . 'class ' . ucfirst($classname) . $nl . '{';

                $classesArr[] = $classname;

                foreach($data_members as $member) {
                    $member = trim($member);
                    if (strlen($member) < 1) {
                        continue;
                    }
                    list($data_type, $member_name) = split(' ', $member);
                    if (!in_array($data_type, $this->simpleTypes)) {
                        $data_type = ucfirst($data_type);
                    }
                    $codeType .= "{$nl}    public \${$member_name}; // -- {$data_type}";
                }

                // -- Classes de mapeamento de dados
                $codeType .= $nl . '}' . $nl;
            }
        }

        $className = get_class($this);
        $mapstr = <<<PHP
<?php
/**
 * Classes de mapeamento para o acesso ao serviço {$className}.
 * @version \$Id\$
 */

/**
 * Classe de definição de mapeamentos do webservice.
 * @see {$className}
 */
class {$className}Map
{
    public static function getClassmap()
    {
        return self::\$classmap;
    }

    private static \$classmap = array(\n
PHP;

        $classMAPCode = array();
        foreach($classesArr as $cname) {

            $cname2 = ucfirst($cname);

            $classMAPCode[] = "        '$cname'=>'$cname2',";
        }

        $mapstr .= implode ("\n",$classMAPCode);
        $mapstr .= "\n    );\n}";

        return "{$mapstr}\n{$codeType}";
    }

    /**
     * Cria uma lista de todos os métodos do webservice.
     * Método adaptado de EasyWsdl2PHP::generate()
     */
    final public function getWSDLFunctions()
    {
        $functions = $this->soapClient->__getFunctions();
        $nl = "\n";
        $code = '';

        foreach($functions as $func) {
            $temp = split(' ', $func, 2);

            //less process whateever is inside ()
            $start = strpos($temp[1], '(');
            $end = strpos($temp[1], '(');
            $parameters = substr($temp[1], $start, $end);

            $t1 = str_replace(')', '', $temp[1]);
            $t1 = str_replace('(', ':', $t1);
            $t2 = split(':', $t1);
            $func = $t2[0];
            $par = $t2[1];

            $params = split(' ', $par);
            $p1 = '$' . $params[0];

            $code .= $nl . '    public function ' . $func . '(' . $p1 .')' . "{$nl}    {\n";
            if ($temp[0] == 'void') {
                $code .=  $nl . "        \$this->soapClient->call('$func', array({$p1}));{$nl}    }{$nl}";
            } else {
                $code .= '        return ' .  "\$this->soapClient->call('$func', array({$p1}));{$nl}    }{$nl}";
            }
        }
        return $code;
    }

    /**
     * Cria as opções de conexão baseadas em ambiente de
     * execução do webservice.
     * @see ProgramacaoFinanceira
     *
     * @return \Simec_SoapClient_Options
     */
    abstract protected function loadConnectionOptions();

    /**
     * Cria o array de mapeamento de classes conforme necessidade
     * do serviço que será consumido. Se não for usar o modelo
     * de classmap, basta implementar a função retornando null.
     *
     * @return \Simec_SoapClient_ClassMap|null
     */
    abstract protected function loadClassMap();

    /**
     * Inicializações pré-constructor.
     * Use para iniciar a URL de conexão, e outras tarefas que precisa ser feitas,
     * antes de construtir o SoapClient.
     */
    abstract protected function _init();
}
